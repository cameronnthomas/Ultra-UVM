#include <uvm>
#include <systemc>

#include "process.h"
#include <iostream>
#include <vector>
#include <string>

using namespace uvm;

/*============AGENT============*/
class vip_agent : public uvm_agent
{
public:
    vip_sequencer<vip_trans> *sequencer;
    vip_driver<vip_trans> *driver;
    vip_monitor *monitor;

    vip_agent(uvm_component_name name)
        : uvm_agent(name), sequencer(0), driver(0), monitor(0) {}
    UVM_COMPONENT_UTILS(vip_agent);
    virtual void build_phase(uvm_phase &phase)
    {
        uvm_agent::build_phase(phase);
        if (get_is_active() == UVM_ACTIVE)
        {
            sequencer = vip_sequencer<vip_trans>::type_id::create("sequencer", this);
            assert(sequencer);
            driver = vip_driver<vip_trans>::type_id::create("driver", this);
            assert(driver);
        }
        monitor = vip_monitor::type_id::create("monitor", this);
        assert(monitor);
    }
    virtual void connect_phase(uvm_phase &phase)
    {
        if (get_is_active() == UVM_ACTIVE)
            driver->seq_item_port.connect(sequencer->seq_item_export);
    }
};