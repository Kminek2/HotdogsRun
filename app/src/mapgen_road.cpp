#include "mapgen_road.hpp"

using mapgen::direction;

namespace mapgen {
    __road createRoadMap(std::string type) {
        __road roadMap;

        // ========= STRAIGHT PIECES =========
        // Cardinal opposites use "prosta"
        roadMap[{direction::N, direction::S}] = new GameObject("prosta" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::E, direction::W}] = new GameObject("prosta" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

        // Diagonal opposites use "prostySkos"
        roadMap[{direction::NE, direction::SW}] = new GameObject("prostySkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::NW, direction::SE}] = new GameObject("prostySkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

        // ========= 90° TURNS =========
        // Among cardinal directions:
        roadMap[{direction::N, direction::E}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::E, direction::S}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::S, direction::W}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::W, direction::N}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

        // Among diagonal directions (90° difference):
        roadMap[{direction::NE, direction::SE}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::SE, direction::SW}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::SW, direction::NW}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::NW, direction::NE}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

        // ========= 45° TURNS (Acute) =========
        // Cardinal -> Diagonal (where the turn is only 45°)
        roadMap[{direction::N, direction::NE}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::NE, direction::N}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        roadMap[{direction::E, direction::SE}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::SE, direction::E}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        roadMap[{direction::S, direction::SW}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::SW, direction::S}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        roadMap[{direction::W, direction::NW}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::NW, direction::W}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        // ========= 135° TURNS (Obtuse) =========
        // Cardinal -> Diagonal (non-adjacent; 135° turn)
        roadMap[{direction::N, direction::NW}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::NW, direction::N}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        roadMap[{direction::E, direction::NE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::NE, direction::E}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        roadMap[{direction::S, direction::SE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::SE, direction::S}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        roadMap[{direction::W, direction::SW}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
        roadMap[{direction::SW, direction::W}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

        return roadMap;
    }
}
