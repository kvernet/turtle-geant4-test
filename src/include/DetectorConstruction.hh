#include "G4VUserDetectorConstruction.hh"
#include "G4Turtle.hh"
#include "globals.hh"

struct DetectorConstruction : public G4VUserDetectorConstruction {
    DetectorConstruction(
            const G4String& model="share/topography/DEM_BACKGROUND_MASAYA.png",
            const G4bool& shadowVolume=false);
    ~DetectorConstruction() override = default;

    /* Singleton */
    static DetectorConstruction *Singleton(
            const G4String& model, const G4bool& shadowVolume=false);
    
    G4VPhysicalVolume *Construct() override;
    
    G4LogicalVolume * ShadowLogicalVolume();
    
    G4String topoModel;
    
    G4Turtle *turtle = nullptr;
    G4PVPlacement * shadowVPlacement = nullptr;
    G4bool addShadowVolume = false;
};
