#include "StackingAction.hh"


StackingAction::StackingAction(const DetectorConstruction* det)
{
  local_det = det;
}

StackingAction::~StackingAction()
{}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* currentTrack)
{
  // if a new track is created beyond interogation material kill it
  G4double EndIntObj = local_det->getEndIntObj();
  G4double trackZ = currentTrack->GetPosition().z();
  if(trackZ/(cm) > EndIntObj/(cm)) return fKill;
  G4ParticleDefinition *pdef = currentTrack->GetDefinition();
  // kill neutrons (probably not important)
  if(pdef == G4Neutron::Definition()) return fKill;
  return fUrgent;
}
