#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGenerator.hh"
#include "SteppingAction.hh"
#include "G4Turtle.hh"
#include "turtle.h"

#include "G4RunManagerFactory.hh"

using namespace std;

struct rock {
    double azimuth;
    double elevation;
    double depth;
};

int main(int argc, char **argv) {
    G4cout << "=== main()" << G4endl;
    
    const G4String modelPath("share/topography/DEM_BACKGROUND_MASAYA.png");
    const G4double azimuthMin = -180;
    const G4double azimuthMax = 180;
    const G4double elevationMin = 0;
    const G4double elevationMax = 90;
    const G4double resolution = 1;
    const G4String outputFile("rock.turtle");
    
    
    /* Construct the default run manager */
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    
    /* Set mandatory initialization classes */
    runManager->SetUserInitialization(DetectorConstruction::Singleton(modelPath));
    auto && physics = PhysicsList::Singleton();
    runManager->SetUserInitialization(physics);
    physics->DisableVerbosity();
    
    auto && primary = PrimaryGenerator::Singleton();
    runManager->SetUserAction(primary);
    auto && action = SteppingAction::Singleton();
    runManager->SetUserAction(action);
    
    runManager->Initialize();

    const G4int nAzimuth   = std::floor((azimuthMax - azimuthMin) / resolution) + 1;
    const G4int nElevation = std::floor((elevationMax - elevationMin) / resolution) + 1;
    
    /* Position */
    double latitude = 11.988726;
    double longitude = -86.171573;
    double height = -100;
    
    G4ThreeVector position = G4Turtle::GetInstance()->GetECEFPosition(
            latitude, longitude, height*CLHEP::m);
    
    /* Energy */
    const G4double energy = 1*CLHEP::GeV;
    const int n = 1;
    
    const double RAD = M_PI / 180;
    
    FILE *stream = fopen(outputFile, "w");
    if(stream == nullptr) {
        G4cout << "Could not create file " << outputFile << G4endl;
        return EXIT_FAILURE;
    }
    
    for(int i = 0; i < nAzimuth; i++) {
        const double azimuth = azimuthMin + i * resolution;
        for(int j = 0; j < nElevation; j++) {
            const double elevation = elevationMin + j * resolution;
            
            G4ThreeVector direction = G4Turtle::GetInstance()->GetECEFDirection(
                    latitude, longitude,
                    azimuth*CLHEP::deg, elevation*CLHEP::deg);
            primary->PrimarySetState(position, direction, energy);
            
            action->totalLength = 0.;
            /* Run the simulation */
            runManager->BeamOn(n);
            
            /* Compute totalLength mean */
            
            action->totalLength /= n;
            
            struct rock r = {
                azimuth, elevation, action->totalLength / CLHEP::m
            };
            
            fwrite(&r, sizeof(r), 1, stream);
        }
    }
    
    
    fclose(stream);
    
    delete runManager;
    
    return EXIT_SUCCESS;
}
