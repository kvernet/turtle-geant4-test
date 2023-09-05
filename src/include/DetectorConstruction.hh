#include "G4VUserDetectorConstruction.hh"
#include "G4Turtle.hh"
#include "globals.hh"

struct DetectorConstruction : public G4VUserDetectorConstruction {
    DetectorConstruction(const G4String& model="share/topography/DEM_BACKGROUND_MASAYA.png");
    ~DetectorConstruction() override = default;

    /* Singleton */
    static DetectorConstruction *Singleton(const G4String& model);
    
    G4VPhysicalVolume *Construct() override;
    
    G4String topoModel;
    G4Turtle *turtle = nullptr;
};
