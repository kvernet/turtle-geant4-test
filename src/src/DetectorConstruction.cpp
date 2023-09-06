#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4NistManager.hh"


DetectorConstruction *DetectorConstruction::Singleton(const G4String& model) {
    static DetectorConstruction *detectorConstruction = nullptr;
    if(detectorConstruction == nullptr) {
        detectorConstruction = new DetectorConstruction(model);
    }
    return detectorConstruction;
}

DetectorConstruction::DetectorConstruction(const G4String& model) : topoModel(model) {
    /* Fetch the topography */
    this->turtle = G4Turtle::GetInstance();
    this->turtle->SetTopographyData("", topoModel);
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    return this->turtle->GetPhysicalVolume();
}
