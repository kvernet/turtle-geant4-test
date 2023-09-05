#include "G4VUserPhysicsList.hh"
#include "globals.hh"

struct PhysicsList : public G4VUserPhysicsList {
    PhysicsList();
    ~PhysicsList() override;
    
    /* Mutable data. */
    G4String model;
    
    /* Singleton. */
    static PhysicsList *Singleton();
    
    /* Disable physics verbosity. */
    void DisableVerbosity() const;
    
    void ConstructParticle();
    void ConstructProcess();
};
