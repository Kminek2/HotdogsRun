#include "mapgen_road.hpp"

using mapgen::direction;

namespace mapgen {
    __road createRoadMap(std::string type) {
        __road roadMap;

        // ========= STRAIGHT PIECES =========
        // Cardinal opposites use "prosta"
        roadMap[{direction::N, direction::S}] = new ObjectSchema("prosta" + type, { 0,0,0 }); // N-S, brak obrotu
        roadMap[{direction::E, direction::W}] = new ObjectSchema("prosta" + type, { 0,0,90 }); // E-W, obrót o 90° wokó³ Z

        // Diagonal opposites use "prostySkos"
        roadMap[{direction::NE, direction::SW}] = new ObjectSchema("prostySkos" + type, { 0,0,90 }); // NE-SW, obrót o 90° wokó³ Z
        roadMap[{direction::NW, direction::SE}] = new ObjectSchema("prostySkos" + type, { 0,0,-90 }); // NW-SE, obrót o -90° wokó³ Z

        // ========= 90° TURNS =========
        // Among cardinal directions:
        roadMap[{direction::N, direction::E}] = new ObjectSchema("zakret" + type, { 0,0,90 }); // N-E, obrót o 90° wokó³ Z
        roadMap[{direction::E, direction::S}] = new ObjectSchema("zakret" + type, { 0,0,180 }); // E-S, obrót o 180° wokó³ Z
        roadMap[{direction::S, direction::W}] = new ObjectSchema("zakret" + type, { 0,0,270 }); // S-W, obrót o 270° wokó³ Z
        roadMap[{direction::W, direction::N}] = new ObjectSchema("zakret" + type, { 0,0,0 }); // W-N, brak obrotu

        // Among diagonal directions (90° difference):
        roadMap[{direction::NE, direction::SE}] = new ObjectSchema("zakrestSkos" + type, { 0,0,90 }); // NE-SE, obrót o 90° wokó³ Z
        roadMap[{direction::SE, direction::SW}] = new ObjectSchema("zakrestSkos" + type, { 0,0,180 }); // SE-SW, obrót o 180° wokó³ Z
        roadMap[{direction::SW, direction::NW}] = new ObjectSchema("zakrestSkos" + type, { 0,0,270 }); // SW-NW, obrót o 270° wokó³ Z
        roadMap[{direction::NW, direction::NE}] = new ObjectSchema("zakrestSkos" + type, { 0,0,0 }); // NW-NE, brak obrotu

        // ========= 45° TURNS (Acute) =========
        // Cardinal -> Diagonal (where the turn is only 45°)
        roadMap[{direction::N, direction::NE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,45 }); // N-NE, obrót o 45° wokó³ Z
        roadMap[{direction::E, direction::NE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,0 }); // E-NE, brak obrotu
        roadMap[{direction::E, direction::SE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,90 }); // E-SE, obrót o 90° wokó³ Z
        roadMap[{direction::S, direction::SE}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,135 }); // S-SE, obrót o 135° wokó³ Z
        roadMap[{direction::S, direction::SW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,180 }); // S-SW, obrót o 180° wokó³ Z
        roadMap[{direction::W, direction::SW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,225 }); // W-SW, obrót o 225° wokó³ Z
        roadMap[{direction::W, direction::NW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,270 }); // W-NW, obrót o 270° wokó³ Z
        roadMap[{direction::N, direction::NW}] = new ObjectSchema("zakretSkosOstry" + type, { 0,0,315 }); // N-NW, obrót o 315° wokó³ Z

        // ========= 135° TURNS (Obtuse) =========
        // Cardinal -> Diagonal (non-adjacent; 135° turn)
        roadMap[{direction::N, direction::SW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,135 }); // N-SW, obrót o 135° wokó³ Z
        roadMap[{direction::N, direction::SE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,45 }); // N-SE, obrót o 45° wokó³ Z
        roadMap[{direction::E, direction::NW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,315 }); // E-NW, obrót o 315° wokó³ Z
        roadMap[{direction::E, direction::SW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,225 }); // E-SW, obrót o 225° wokó³ Z
        roadMap[{direction::S, direction::NE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,45 }); // S-NE, obrót o 45° wokó³ Z
        roadMap[{direction::S, direction::NW}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,315 }); // S-NW, obrót o 315° wokó³ Z
        roadMap[{direction::W, direction::NE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,135 }); // W-NE, obrót o 135° wokó³ Z
        roadMap[{direction::W, direction::SE}] = new ObjectSchema("zakretPolSkos" + type, { 0,0,225 }); // W-SE, obrót o 225° wokó³ Z

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