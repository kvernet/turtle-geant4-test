#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4NistManager.hh"


DetectorConstruction *DetectorConstruction::Singleton(
        const G4String& model, const G4bool& shadowVolume) {
    static DetectorConstruction *detectorConstruction = nullptr;
    if(detectorConstruction == nullptr) {
        detectorConstruction = new DetectorConstruction(model, shadowVolume);
    }
    return detectorConstruction;
}

DetectorConstruction::DetectorConstruction(
        const G4String& model, const G4bool& shadowVolume) :
        topoModel(model), addShadowVolume(shadowVolume) {
    /* Fetch the topography */
    this->turtle = G4Turtle::GetInstance();
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    this->turtle->SetTopographyData("", this->topoModel);
    
    if(this->addShadowVolume) {
        G4LogicalVolume *shadowLogicalVolume = this->ShadowLogicalVolume();
        
        G4double latitude = 11.988;
        G4double longitude = -86.165;
        G4double height = -100 * CLHEP::m;
        G4double azimuth = 0 * CLHEP::deg;
        G4double zenith = 90 * CLHEP::deg;
        G4double intrinsic = 0 * CLHEP::deg;
        const G4String name = "ShadowBox";
        
        shadowVPlacement = this->turtle->PVPlacement(
                latitude, longitude, height,
                azimuth, zenith, intrinsic,
                shadowLogicalVolume, name,
                false, 0);
    }
    return this->turtle->GetPhysicalVolume();
}

G4LogicalVolume *DetectorConstruction::ShadowLogicalVolume() {
    const G4double volumeSize[] = {
            100*CLHEP::m, 100*CLHEP::m, 100*CLHEP::m
    };
    
    auto shadowBox = new G4Box("ShadowBox",
            0.5 * volumeSize[0], 0.5 * volumeSize[1],
            0.5 * volumeSize[2]);
    
    G4NistManager *manager = G4NistManager::Instance();
    G4Material *shadowMaterial = manager->FindOrBuildMaterial("G4_AIR");
    
    return new G4LogicalVolume(shadowBox, shadowMaterial, "shadowMaterial");
}
