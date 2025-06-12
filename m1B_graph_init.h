
#include "m1A_constants.h"

namespace M1Env{

// [FOLDR] Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)
extern M1Env::SpecialItemID FOLDER_SIID;
// [AUTO_] Special Vertex ID (Edge type) of AUTO edges
extern M1Env::SpecialItemID AUTO_SIID;
// [OWNS_] Special Vertex ID (Edge type) of ownership edges
extern M1Env::SpecialItemID OWNS_SIID;
// [BLNGS] Special Vertex ID (Edge type) of ownership edges (reciprocal of OWNS_ above))
extern M1Env::SpecialItemID BLNGS_SIID;
// [_ISA_] Special Vertex ID (Edge type) of type edges
extern M1Env::SpecialItemID ISA_SIID;
// [_ITO_] Special Vertex ID (Edge type) of type edges (reciprocal of _ISA_ above))
extern M1Env::SpecialItemID ITO_SIID;

// [ROOT_] Special Vertex ID of "Global graph root"
extern M1Env::SpecialItemID ROOT_SIID;
// [HOME_] Special Vertex ID of "Home"
extern M1Env::SpecialItemID HOME_SIID;
// [TYPE_] Special Vertex ID of "Root of all types"
extern M1Env::SpecialItemID TYPE_NODE_SIID;
// [PERSN] Special Vertex ID of "Person (type)"
extern M1Env::SpecialItemID PERS_TYPE_SIID;
// [ORGN_] Special Vertex ID of "Organization (type)"
extern M1Env::SpecialItemID ORG_TYPE_SIID;
// [ME___] Special Vertex ID of "Me"
extern M1Env::SpecialItemID ME_SIID;
// [_MSG_] Special Vertex ID of "Inboxes / Message Root Type"
extern M1Env::SpecialItemID MSG_TYPE_SIID;
// [EMAIL] Special Vertex ID of "Email Inbox"
extern M1Env::SpecialItemID EMAIL_TYPE_SIID;
// [WHTSP] Special Vertex ID of "Whatsapp Inbox"
extern M1Env::SpecialItemID WHTSP_TYPE_SIID;
// [DSCRD] Special Vertex ID of "Discord Inbox"
extern M1Env::SpecialItemID DSCRD_TYPE_SIID;
// [GCHAT] Special Vertex ID of "GChat Inbox"
extern M1Env::SpecialItemID GCHAT_TYPE_SIID;
// [SMS__] Special Vertex ID of "SMS Inbox"
extern M1Env::SpecialItemID SMS_TYPE_SIID;


    class GraphInit{
        public:
            static void set_pseudo_constants();
            static void init_base();
            static void init_text();
    };
    
} // end namespace M1Env
