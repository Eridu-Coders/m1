#ifndef NAV_H
#define NAV_H

#include "m1B_lv2_item.h"

namespace M1Store{

    class Nav{
    private:
        Item_lv2* m_origin;
        Item_lv2* m_current_edge;

        //! only set after a call to next(), if m_current_edge is on the m_first_edge of m_origin
        bool m_is_at_end = false;
    public:
        Nav(SpecialItemID p_id_special);

        void next();
        void previous();
        // void next_until(SpecialItemID p_type_id_special);
        // void previous_until(SpecialItemID p_type_id_special);

        bool origin_is_edge();
        bool origin_is_of_type(SpecialItemID p_type_id_special);
        bool edge_is_of_type(SpecialItemID p_type_id_special);
        bool is_at_end();

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();
    };
}

    QDebug operator<<(QDebug d, M1Store::Nav p);
    QDebug operator<<(QDebug d, M1Store::Nav* p);

#endif // NAV_H
