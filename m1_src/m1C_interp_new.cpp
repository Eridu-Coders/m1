#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_iterators.h"
#include "m1D_tree_display.h"
#include "m1D_passages_panel.h"

#include <QPainter>
#include <QResizeEvent>
#include <QRegularExpression>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>

// g_cat_interp_dev
// Q_LOGGING_CATEGORY(g_cat_interp_dev, "interp.dev")
// Q_LOGGING_CATEGORY(g_cat_interp_base, "interp.base")
// g_cat_interp_drag
// Q_LOGGING_CATEGORY(g_cat_interp_drag, "interp.drag")

// QIcon M1MidPlane::Interp::cm_open;
// QIcon M1MidPlane::Interp::cm_closed;
