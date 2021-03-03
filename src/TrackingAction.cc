#include "Trajectory.hh"
#include "TrackingAction.hh"
#include "UserTrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction()
:photonsOnly(false),opticalOnly(false),detectedOnly(false),chopperOnly(false),
trackM(NULL)
{
  trackM = new TrackingMessenger(this);
}

TrackingAction::~TrackingAction()
{
  delete trackM;
}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new Trajectory(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new UserTrackInformation);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  Trajectory* trajectory =
    (Trajectory*)fpTrackingManager->GimmeTrajectory();
  UserTrackInformation*
    trackInformation=(UserTrackInformation*)aTrack->GetUserInformation();

  // This is where the magic happens
  if(photonsOnly)
  {
    if(aTrack->GetDefinition()==G4Gamma::Definition()
      || aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
    {
      trajectory->SetDrawTrajectory(true);
    }
  }
  else if(opticalOnly)
  {
    if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
      trajectory->SetDrawTrajectory(true);
  }
  else if(detectedOnly)
  {
    G4String volumeName = aTrack->GetVolume()->GetName();
    if(volumeName.compare(0,2,"PC"))
      trajectory->SetDrawTrajectory(true);
  }
  else if(chopperOnly)
  {
    G4String volumeName = aTrack->GetVolume()->GetName();
    if(volumeName.compare(0,4,"Chop"))
      trajectory->SetDrawTrajectory(true);
  }
  else
    // Draw all tracks
    trajectory->SetDrawTrajectory(true);

  if(trackInformation->GetForceDrawTrajectory())
    trajectory->SetDrawTrajectory(true);
}
