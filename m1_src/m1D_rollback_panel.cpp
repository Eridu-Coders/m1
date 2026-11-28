#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QLoggingCategory>

#include "m1D_rollback_panel.h"

Q_LOGGING_CATEGORY(g_cat_rollback_panel, "rollback_panel")

namespace {
    const int ROLE_INDEX = Qt::UserRole;        ///< transaction index (0 = oldest), top-level items only
    const int ROLE_SEQ = Qt::UserRole + 1;      ///< transaction sequence number
    const int ROLE_OPEN_CLOSE = Qt::UserRole + 2;
}

M1UI::RollbackPanel::RollbackPanel(QWidget* p_parent) : QWidget(p_parent){
    QVBoxLayout* l_layout = new QVBoxLayout(this);

    QHBoxLayout* l_bar = new QHBoxLayout();
    m_show_open_close = new QCheckBox("open/close", this);
    m_show_open_close->setChecked(false);
    m_show_open_close->setToolTip("Show the tree open/close transactions");
    l_bar->addWidget(m_show_open_close);
    l_bar->addStretch(1);
    m_selection_label = new QLabel(this);
    l_bar->addWidget(m_selection_label);
    m_rollback_btn = new QPushButton("Rollback", this);
    m_rollback_btn->setEnabled(false);
    l_bar->addWidget(m_rollback_btn);
    l_layout->addLayout(l_bar);

    m_tree = new QTreeWidget(this);
    m_tree->setColumnCount(3);
    m_tree->setHeaderLabels({"Date-time", "Transaction", "Summary"});
    m_tree->setUniformRowHeights(false);
    m_tree->setWordWrap(false);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_tree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_tree->header()->setStretchLastSection(true);
    l_layout->addWidget(m_tree);

    connect(m_tree, &QTreeWidget::itemChanged, this, &RollbackPanel::itemChanged);
    connect(m_tree, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem* p_item){m_expanded.insert(p_item->data(0, ROLE_SEQ).toULongLong());});
    connect(m_tree, &QTreeWidget::itemCollapsed, this, [this](QTreeWidgetItem* p_item){m_expanded.remove(p_item->data(0, ROLE_SEQ).toULongLong());});
    connect(m_show_open_close, &QCheckBox::toggled, this, &RollbackPanel::rebuild);
    connect(m_rollback_btn, &QPushButton::clicked, this, [this](){
        if(m_select_from >= 0) emit rollbackRequested(m_count - m_select_from);
    });
    // queued: the notification comes from within commit() / rollback()
    connect(M1Store::Archive::notifier(), &M1Store::ArchiveNotifier::logChanged, this, &RollbackPanel::rebuild, Qt::QueuedConnection);

    rebuild();
}

/**
 * @brief rebuild the list from the archive log index (descriptions are cached by sequence number)
 */
void M1UI::RollbackPanel::rebuild(){
    const int l_count = M1Store::Archive::transactionCount();
    qCDebug(g_cat_rollback_panel) << "rebuild" << l_count << "transaction(s)";

    // the selection does not survive a change in the list (new commit or rollback)
    const quint64 l_last_seq = l_count > 0 ? M1Store::Archive::transactionSeq(l_count - 1) : 0;
    if(l_count != m_count || l_last_seq != m_last_seq) m_select_from = -1;
    m_count = l_count;
    m_last_seq = l_last_seq;

    // drop the descriptions of transactions no longer in the log
    QSet<quint64> l_present;
    for(int i = 0; i < l_count; i++) l_present.insert(M1Store::Archive::transactionSeq(i));
    for(auto l_it = m_cache.begin(); l_it != m_cache.end();)
        if(!l_present.contains(l_it.key())) l_it = m_cache.erase(l_it); else ++l_it;

    m_updating = true;
    m_tree->clear();
    const bool l_show_oc = m_show_open_close->isChecked();
    for(int i = l_count - 1; i >= 0; i--){
        const quint64 l_seq = M1Store::Archive::transactionSeq(i);
        if(!m_cache.contains(l_seq)) m_cache.insert(l_seq, M1Store::Archive::describeTransaction(i));
        const M1Store::TxDescription& l_desc = m_cache[l_seq];

        QTreeWidgetItem* l_item = new QTreeWidgetItem();
        l_item->setText(0, l_desc.m_ok ? QDateTime::fromMSecsSinceEpoch(l_desc.m_timestamp_ms).toString("yyyy-MM-dd HH:mm:ss") : QString("?"));
        l_item->setText(1, l_desc.m_label);
        l_item->setText(2, l_desc.m_summary);
        l_item->setToolTip(2, l_desc.m_summary);
        l_item->setData(0, ROLE_INDEX, i);
        l_item->setData(0, ROLE_SEQ, l_seq);
        l_item->setData(0, ROLE_OPEN_CLOSE, l_desc.m_open_close);
        l_item->setFlags((l_item->flags() | Qt::ItemIsUserCheckable) & ~Qt::ItemIsAutoTristate);
        l_item->setCheckState(0, Qt::Unchecked);
        for(const QString& l_line : l_desc.m_details){
            QTreeWidgetItem* l_child = new QTreeWidgetItem(l_item);
            l_child->setText(0, l_line);
            l_child->setToolTip(0, l_line);
            l_child->setData(0, ROLE_SEQ, l_seq);
            l_child->setFirstColumnSpanned(true);
        }
        m_tree->addTopLevelItem(l_item);
        for(int k = 0; k < l_item->childCount(); k++) l_item->child(k)->setFirstColumnSpanned(true);
        l_item->setHidden(l_desc.m_open_close && !l_show_oc);
        if(m_expanded.contains(l_seq)) l_item->setExpanded(true);
    }
    m_updating = false;
    applySelection();
}

/**
 * @brief tick box handler: ticking selects this transaction and all newer ones; unticking deselects it and all older ones
 */
void M1UI::RollbackPanel::itemChanged(QTreeWidgetItem* p_item, int p_column){
    if(m_updating || p_column != 0 || p_item->parent() != nullptr) return;
    const int l_index = p_item->data(0, ROLE_INDEX).toInt();
    if(p_item->checkState(0) == Qt::Checked) m_select_from = l_index;
    else m_select_from = (l_index + 1 < m_count) ? l_index + 1 : -1;
    applySelection();
}

/// set the tick boxes and the selection label according to m_select_from
void M1UI::RollbackPanel::applySelection(){
    m_updating = true;
    int l_selected = 0, l_hidden = 0;
    for(int k = 0; k < m_tree->topLevelItemCount(); k++){
        QTreeWidgetItem* l_item = m_tree->topLevelItem(k);
        const bool l_on = m_select_from >= 0 && l_item->data(0, ROLE_INDEX).toInt() >= m_select_from;
        l_item->setCheckState(0, l_on ? Qt::Checked : Qt::Unchecked);
        if(l_on){
            l_selected++;
            if(l_item->isHidden()) l_hidden++;
        }
    }
    m_updating = false;

    if(l_selected == 0) m_selection_label->setText(m_count == 0 ? QString("no transaction in this session") : QString());
    else{
        QString l_text = QString("%1 transaction%2 selected").arg(l_selected).arg(l_selected > 1 ? "s" : "");
        if(l_hidden > 0) l_text += QString(" (incl. %1 hidden open/close)").arg(l_hidden);
        m_selection_label->setText(l_text);
    }
    m_rollback_btn->setEnabled(l_selected > 0);
}
