#include "SteppingAction.hh"
#include "G4Turtle.hh"

SteppingAction::SteppingAction() {}

SteppingAction *SteppingAction::Singleton() {
    static SteppingAction *action = nullptr;
    if(action == nullptr) {
        action = new SteppingAction;
    }
    return action;
}
void SteppingAction::UserSteppingAction(const G4Step *step) {
    auto && material = step->GetPreStepPoint()->GetMaterial();
    
    if(material == G4Turtle::GetInstance()->GetRockMaterial()) {
        totalLength += step->GetStepLength();
    }
}
