#ifndef M1D_ROLLBACK_PANEL_H
#define M1D_ROLLBACK_PANEL_H

#include <QWidget>
#include <QHash>
#include <QSet>

#include "m1B_archive.h"

class QTreeWidget;
class QTreeWidgetItem;
class QCheckBox;
class QPushButton;
class QLabel;

namespace M1UI{

/**
 * @brief "Rollback" tab (right panel): list of the transactions committed during the current session (newest first),
 * with date-time, label, summary and (expandable) details. Ticking a transaction selects it and all newer ones
 * (only a suffix of the log can be undone); the Rollback button undoes the selected transactions.
 *
 * Open/close edge transactions are hidden unless the "open/close" box is ticked; hidden transactions inside the selected
 * range are rolled back as well (and counted in the selection label).
 */
class RollbackPanel : public QWidget{
    Q_OBJECT
private:
    QCheckBox* m_show_open_close;
    QLabel* m_selection_label;
    QPushButton* m_rollback_btn;
    QTreeWidget* m_tree;

    QHash<quint64, M1Store::TxDescription> m_cache;     ///< descriptions by transaction sequence number
    QSet<quint64> m_expanded;                           ///< expanded transactions (kept across rebuilds)
    int m_select_from = -1;     ///< index (0 = oldest) of the oldest selected transaction, -1 = none selected
    int m_count = 0;            ///< number of transactions displayed
    quint64 m_last_seq = 0;     ///< sequence number of the newest transaction displayed
    bool m_updating = false;    ///< re-entrance guard (programmatic check state changes)

    void applySelection();
public:
    explicit RollbackPanel(QWidget* p_parent = nullptr);
public slots:
    void rebuild();
private slots:
    void itemChanged(QTreeWidgetItem* p_item, int p_column);
signals:
    /// the user confirmed nothing yet: MainWindow asks for confirmation, then performs the rollback
    void rollbackRequested(int p_count);
};

} // namespace M1UI

#endif // M1D_ROLLBACK_PANEL_H
