#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

struct PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
    PrimaryGenerator();
    ~PrimaryGenerator() override;
    
    /* Singleton. */
    static PrimaryGenerator *Singleton();
    void GeneratePrimaries(G4Event*) override;
    
    void PrimarySetState(const G4ThreeVector position,
            const G4ThreeVector direction, const G4double energy=1*CLHEP::GeV);
    
    G4ParticleGun *particleGun = nullptr;
};
