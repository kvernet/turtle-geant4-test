#include "PrimaryGenerator.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"

#include "G4MuonMinus.hh"


PrimaryGenerator::PrimaryGenerator() : G4VUserPrimaryGeneratorAction() {
    const G4int n_particle = 1;
    this->particleGun = new G4ParticleGun(n_particle);
}
PrimaryGenerator::~PrimaryGenerator() {
    delete this->particleGun;
}
PrimaryGenerator *PrimaryGenerator::Singleton() {
    static PrimaryGenerator *generator = nullptr;
    if(generator == nullptr) {
        generator = new PrimaryGenerator;
    }
    return generator;
}
void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    this->particleGun->SetParticleDefinition(G4Geantino::GeantinoDefinition());
    
    // Create primary vertex.
    this->particleGun->GeneratePrimaryVertex(anEvent);
}
void PrimaryGenerator::PrimarySetState(const G4ThreeVector position,
        const G4ThreeVector direction, const G4double energy) {
    this->particleGun->SetParticleMomentumDirection(direction);
    this->particleGun->SetParticlePosition(position);
    this->particleGun->SetParticleEnergy(energy);
}
