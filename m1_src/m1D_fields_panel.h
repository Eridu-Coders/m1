#ifndef M1D_FIELDS_PANEL_H
#define M1D_FIELDS_PANEL_H

#include <QWidget>
#include <QList>

#include "m1B_lv2_item.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QGridLayout;
class QScrollArea;

namespace M1UI{

/**
 * @brief The "fields" tab of the right panel: a form to edit the label and the fields of the vertex highlighted in the left tree
 *
 * Fields are either simple edges (text stored in the edge) or full edges leading to a simple vertex (text stored in the vertex).
 * All existing fields are listed with an editable value. New (simple edge) fields can be added through the combo
 * (types with SI_FIELD_TYPE set) and the "Add" button. Nothing is written to the store until "Save" is clicked.
 * Saving an empty value removes the field (Item_lv2::unlinkEdge(), see edge deletion policy).
 */
class FieldsPanel : public QWidget{
    Q_OBJECT
private:
    /// one row of the form
    struct FieldRow{
        M1Store::Item_lv2* m_edge = nullptr;                    ///< field edge (nullptr --> new field, not yet saved)
        M1Env::SpecialItemID m_type = M1Env::G_VOID_SI_ID;      ///< field type (used for new fields)
        QLineEdit* m_editor = nullptr;                          ///< value editor
        QString m_original;                                     ///< value when the form was built
    };

    M1Store::Item_lv2* m_vertex = nullptr;      ///< vertex whose fields are being edited

    QLineEdit* m_label_edit = nullptr;         ///< vertex label editor (top of the panel)
    QString m_label_original;                  ///< vertex label when the form was built
    QScrollArea* m_scroll = nullptr;
    QWidget* m_form_widget = nullptr;
    QGridLayout* m_form_layout = nullptr;
    QComboBox* m_type_combo = nullptr;
    QPushButton* m_add_btn = nullptr;
    QPushButton* m_save_btn = nullptr;
    QPushButton* m_revert_btn = nullptr;
    QLabel* m_message = nullptr;

    QList<FieldRow> m_rows;

    void rebuildForm();
    void clearForm();
    void addFormRow(const FieldRow& p_row, const QString& p_type_text);
    void setDirty(bool p_dirty);

    static const M1Store::SpecialItem* fieldTypeOf(const M1Store::Item_lv2* p_edge);
    static bool isFieldEdge(const M1Store::Item_lv2* p_edge);
public:
    explicit FieldsPanel(QWidget* p_parent = nullptr);
    M1Store::Item_lv2* vertex() const {return m_vertex;}
public slots:
    void setVertex(M1Store::Item_lv2* p_vertex);
private slots:
    void addField();
    void save();
    void revert();
signals:
    /// emitted after changes have been written to the store (so that the tree can be refreshed)
    void fieldsSaved(M1Store::Item_lv2* p_vertex);
};

} // namespace M1UI

#endif // M1D_FIELDS_PANEL_H
