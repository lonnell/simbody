#ifndef IVM_SIMBODY_INTERFACE_REP_H_
#define IVM_SIMBODY_INTERFACE_REP_H_

/** @file
 *
 * This is a Simbody-compatible interface to the IVM-derived rigid body
 * simulation toolset.
 */

#include "simmatrix/BigMatrix.h"
#include "simbody/Simbody.h"
#include "simbody/IVMSimbodyInterface.h"

#include "RigidBodyTree.h"

using namespace simtk;

#include <iostream>
#include <vector>

class TreeMap {
public:
    TreeMap() : body(0), frame_BR(), frame_RJ(), 
                parentIndex(-1), joint(0), level(-1), rbIndex(-1) { }
    TreeMap(const Body* b, const Frame& ref, const Frame& jInRef, size_t pix, const Joint* j, int l)
        : body(b), frame_BR(ref), frame_RJ(jInRef), 
          parentIndex(pix), joint(j), level(l), rbIndex(-1) { }

    const Body&  getBody()           const          {assert(body); return *body;}
    const Frame& getRefFrameInBody() const          {assert(body); return frame_BR;}
    void         setRefFrameInBody(const Frame& f)  {frame_BR=f;}
    const Frame& getJointFrameInRef() const         {assert(body); return frame_RJ;}
    void         setJointFrameInRef(const Frame& f) {frame_RJ=f;}

    size_t       getParentIndex() const {assert(parentIndex != size_t(-1)); return parentIndex;}
    const Joint& getJoint()  const {assert(joint);  return *joint;}
    int          getLevel()  const {return level;}
    void         setRBIndex(int ix) {rbIndex=ix;}
    int          getRBIndex() const {assert(rbIndex != -1); return rbIndex;}

private:
    const Body*     body;
    Frame           frame_BR;   // reference frame R meas & expr in B; default=I
    Frame           frame_RJ;   // inboard joint frame meas & expr in ref frame R
    size_t          parentIndex;
    const Joint*    joint;
    int             level;
    int             rbIndex;
};


class IVMSimbodyInterfaceRep /* : public RigidBodyMechanicsResource(?) */ {
public:
    IVMSimbodyInterfaceRep(const Multibody&);

    int getNBodies() const;
    int getNParameters() const;
    int getNQ() const;
    int getNU() const;

    void realizeParameters(const State&) const;
    void realizeConfiguration(const State&) const;
    void realizeMotion(const State&) const;
    void realizeReaction(const State&) const;

    void enforceConfigurationConstraints(State&) const;
    void enforceMotionConstraints(State&) const;

    const Vector& getQDot(const State&) const;

    void setMyHandle(IVMSimbodyInterface& h) {handle=&h;}
    const IVMSimbodyInterface& getMyHandle() const {assert(handle); return *handle;}

    const Frame&         getBodyConfiguration(const State&, int body) const;
    const SpatialVector& getBodyVelocity     (const State&, int body) const;
    const SpatialVector& getBodyAcceleration (const State&, int body) const;

    static RBJointType mapToRBJointType(Joint::JointType jt);
private:
    IVMSimbodyInterface* handle;

    Multibody               mbs;  // private copy
    std::vector<TreeMap>    mbs2tree;
    RigidBodyTree           tree;

};



#endif // IVM_SIMBODY_INTERFACE_REP_H_
