#include "PhysicsList.hh"
#include "G4UImanager.hh"

#include "G4Geantino.hh"

PhysicsList *PhysicsList::Singleton() {
    static PhysicsList *physics = nullptr;
    if(physics == nullptr) physics = new PhysicsList;
    
    return physics;
}

PhysicsList::PhysicsList() : G4VUserPhysicsList() {}

PhysicsList::~PhysicsList() {}

void PhysicsList::DisableVerbosity() const {
    auto UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/process/em/verbose 0");
    UImanager->ApplyCommand("/process/had/verbose 0");
}

void PhysicsList::ConstructParticle() {
    //this->decayPhysics->ConstructParticle();
    
    // In this method, static member functions should be called
   // for all particles which you want to use.
   // This ensures that objects of these particle types will be
   // created in the program. 
   
    G4Geantino::GeantinoDefinition();
}

void PhysicsList::ConstructProcess() {
    AddTransportation();
}
