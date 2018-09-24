/* -------------------------------------------------------------------------- *
 * OpenSim Muscollo: MucoConstraint.cpp                                       *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2017 Stanford University and the Authors                     *
 *                                                                            *
 * Author(s): Nicholas Bianco                                                 *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0          *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "MucoConstraint.h"

using namespace OpenSim;
using SimTK::ConstraintIndex;

// ============================================================================
// MucoConstraint
// ============================================================================

MucoConstraint::MucoConstraint() {
    constructProperties();
    if (getName().empty()) setName("constraint");
}

MucoConstraint::MucoConstraint(const std::string& name, 
    const std::vector<MucoBounds>& bounds, 
    const std::vector<std::string>& suffixes) : MucoConstraint() {
    
    setName(name);
    for (int i = 0; i < bounds.size(); ++i) {
        upd_lower_bounds(i) = bounds[i].getLower();
        upd_upper_bounds(i) = bounds[i].getUpper();
    }
    set_suffixes(suffixes);
}

void MucoConstraint::constructProperties() {
    constructProperty_lower_bounds();
    constructProperty_upper_bounds();
    constructProperty_suffixes();
}

void MucoConstraint::calcConstraintErrors(const SimTK::State&,
    SimTK::Vector_<double> out) const  
{}


// ============================================================================
// MucoSimbodyConstraint
// ============================================================================

MucoSimbodyConstraint::MucoSimbodyConstraint() {
    constructProperties();
}

void MucoSimbodyConstraint::constructProperties() {
    constructProperty_constraint_index(0);
    constructProperty_holonomic_only_mode(true);
}

void MucoSimbodyConstraint::initialize(Model& model) const {
    auto& matter = model.updMatterSubsystem();
    auto NC = matter.getNumConstraints();

    const SimTK::State& state = model.initSystem();
    SimTK::Constraint& constraint 
        = matter.updConstraint(ConstraintIndex(get_constraint_index()));

    // Throw error if constraint is not enabled in model.
    // TODO do somewhere else?
    if (constraint.isDisabled(state)) {
        OPENSIM_THROW(Exception, "Constraint is not enabled in model.");
    }

    // Get number of scalar equations of each type?
    constraint.getNumConstraintEquationsInUse(state, m_num_position_eqs, 
        m_num_velocity_eqs, m_num_acceleration_eqs);


    m_num_equations = m_num_position_eqs + m_num_velocity_eqs 
        + m_num_acceleration_eqs;

    m_constraint_ref = constraint;
}

void MucoSimbodyConstraint::calcConstraintErrors(const SimTK::State& state,
    SimTK::Vector errors) const {

    errors = m_constraint_ref->getPositionErrorsAsVector(state);
}