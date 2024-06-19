#include <uvm>
#include <systemc>

#include "process.h"
#include <iostream>
#include <vector>
#include <string>

using namespace uvm;

/*============SCOREBOARD============*/

class scoreboard : public uvm_scoreboard
{
public:
    uvm_analysis_export<vip_trans> xmt_listener_imp;
    uvm_analysis_export<vip_trans> rcv_listener_imp;
    bool error;
    xmt_subscriber *xmt_listener;
    rcv_subscriber *rcv_listener;
    scoreboard(uvm_name name) : uvm_scoreboard(name),
                                xmt_listener_imp("xmt_listener_imp"),
                                rcv_listener_imp("rcv_listener_imp"),
                                xmt_listener(0), rcv_listener(0) {}
    UVM_COMPONENT_UTILS(scoreboard);
    virtual void build_phase(uvm_phase &phase)
    {
        uvm_scoreboard::build_phase(phase);
        
        xmt_listener = xmt_subscriber::type_id::create("xmt_listener", this);
        assert(xmt_listener);
        rcv_listener = rcv_subscriber::type_id::create("rcv_listener", this);
        assert(rcv_listener);
    }
    virtual void connect_phase(uvm_phase &phase)
    {
        xmt_listener_imp.connect(xmt_listener->analysis_export);
        rcv_listener_imp.connect(rcv_listener->analysis_export);
    }
    // virtual void write_xmt(const vip_trans &p) { ... } // store reference information
    // virtual void write_rcv(const vip_trans &p) { ... } // compare received data with reference data
};