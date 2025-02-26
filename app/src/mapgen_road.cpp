#include "mapgen_road.hpp"

using mapgen::direction;

namespace mapgen {
    __road createRoadMap(std::string type) {
        __road roadMap;

        // ========= STRAIGHT PIECES =========
        // Cardinal opposites use "prosta"
        roadMap[{direction::N, direction::S}] = new ObjectSchema("prosta" + type, { 0,0,90 });
        roadMap[{direction::E, direction::W}] = new ObjectSchema("prosta" + type, { 0,0,0 });

        // Diagonal opposites use "prostySkos"
        roadMap[{direction::NE, direction::SW}] = new ObjectSchema("prostySkos" + type, { 0,0,90 });
        roadMap[{direction::NW, direction::SE}] = new ObjectSchema("prostySkos" + type, { 0,0,0 });

        // ========= 90° TURNS =========
        // Among cardinal directions:
        roadMap[{direction::N, direction::E}] = new ObjectSchema("zakret" + type, { 0,0,90 });
        roadMap[{direction::E, direction::S}] = new ObjectSchema("zakret" + type, { 0,0,180 });
        roadMap[{direction::S, direction::W}] = new ObjectSchema("zakret" + type, { 0,0,270 });
        roadMap[{direction::W, direction::N}] = new ObjectSchema("zakret" + type, { 0,0,0 });

        // Among diagonal directions (90° difference):
        roadMap[{direction::NE, direction::SE}] = new ObjectSchema("zakrestSkos" + type, { 0,0,180}, {1,1,1});
        roadMap[{direction::SE, direction::SW}] = new ObjectSchema("zakrestSkos" + type, { 0,0,270}, {1,1,1});
        roadMap[{direction::SW, direction::NW}] = new ObjectSchema("zakrestSkos" + type, { 0,0,0},   {1,1,1});
        roadMap[{direction::NW, direction::NE}] = new ObjectSchema("zakrestSkos" + type, { 0,0,90},  {1,1,1});

        // ========= 45° TURNS (Acute) =========
        // Cardinal -> Diagonal (where the turn is only 45°) (the famous 180° turn)
        roadMap[{direction::N, direction::NE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,270}, {1,-1,1});
        roadMap[{direction::E, direction::NE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,180}, {1,1,1});
        roadMap[{direction::E, direction::SE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,0},   {1,-1,1});
        roadMap[{direction::S, direction::SE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,270}, {1,1,1});
        roadMap[{direction::S, direction::SW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,90},  {1,-1,1});
        roadMap[{direction::W, direction::SW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,0},   {1,1,1});
        roadMap[{direction::W, direction::NW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,0},   {-1,1,1});
        roadMap[{direction::N, direction::NW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,90},  {1,1,1});

        // ========= 135° TURNS (Obtuse) =========
        // Cardinal -> Diagonal (non-adjacent; 135° turn)
        roadMap[{direction::N, direction::SW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,90}, {1,1,1});
        roadMap[{direction::N, direction::SE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,90}, {-1,1,1 });
        roadMap[{direction::E, direction::NW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,180},{1,1,1});
        roadMap[{direction::E, direction::SW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,180},{-1,1,1});
        roadMap[{direction::S, direction::NE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,270},{1,1,1});
        roadMap[{direction::S, direction::NW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,270},{-1,1,1});
        roadMap[{direction::W, direction::NE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,0},  {-1,1,1});
        roadMap[{direction::W, direction::SE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,0},   {1,1,1});

        // ========= 0° TURNS =========
        // Diagonal -> Diagonal (no turn)
        roadMap[{direction::NW, direction::NW}] = new ObjectSchema("arrow");
        roadMap[{direction::SE, direction::SE}] = new ObjectSchema("arrow");
        roadMap[{direction::NE, direction::NE}] = new ObjectSchema("arrow");
        roadMap[{direction::SW, direction::SW}] = new ObjectSchema("arrow");

        // Cardinal -> Cardinal (no turn)
        roadMap[{direction::N, direction::N}] = new ObjectSchema("debug_star");
        roadMap[{direction::E, direction::E}] = new ObjectSchema("debug_star");
        roadMap[{direction::S, direction::S}] = new ObjectSchema("debug_star");
        roadMap[{direction::W, direction::W}] = new ObjectSchema("debug_star");

        return roadMap;
    }
}