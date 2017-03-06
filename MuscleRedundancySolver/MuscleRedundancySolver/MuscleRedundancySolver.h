#ifndef TOMU_MUSCLEREDUNDANCYSOLVER_H
#define TOMU_MUSCLEREDUNDANCYSOLVER_H

#include <OpenSim/OpenSim.h>

namespace OpenSim {

// TODO if this solves for the activity of any actuator, then it should not
// be called "muscle" redundancy solver.
class MuscleRedundancySolver : public OpenSim::Object {
OpenSim_DECLARE_CONCRETE_OBJECT(MuscleRedundancySolver, OpenSim::Object);
public:

    struct Solution {
        TimeSeriesTable excitations;
        TimeSeriesTable activations;
    };

    //OpenSim_DECLARE_PROPERTY(model_file, std::string,
    //    "Path to a model file (.osim).");
    //OpenSim_DECLARE_PROPERTY(kinematics_file, std::string,
    //    "Path to a Storage (.sto) file containing kinematics.");

    MuscleRedundancySolver();

    const Model& getModel() const {
        return _model;
    }
    /// Set the model whose muscles will be used to achieve the desired
    /// motion.
    void setModel(const Model& model) {
        _model = model;
        _model.finalizeFromProperties();
    }

    const TimeSeriesTable& getKinematicsData() const {
        return _kinematics;
    }
    /// Set the kinematics (joint angles) to achieve.
    /// The table should contain generalized coordinates and generalized speeds,
    /// as is generated by StatesTrajectory::exportToTable().
    void setKinematicsData(const TimeSeriesTable& kinematics) {
        _kinematics = kinematics;
    }

    Solution solve();

private:
    Model _model;
    // TODO make this a StatesTrajectory?
    TimeSeriesTable _kinematics;

    // To determine which actuators enter the optimal control problem.
    // TODO std::vector<std::string> _actuatorsToUse;
};

} // namespace OpenSim

#endif // TOMU_MUSCLEREDUNDANCYSOLVER_H
