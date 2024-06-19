#include <uvm>
#include <systemc>

#include "process.h"
#include <iostream>
#include <vector>
#include <string>

using namespace uvm;

/*============DRIVER============*/

template <class REQ>
class vip_driver : public uvm_driver<REQ>
{
public:
    vip_if *vif;
    vip_driver(uvm_name name) : uvm_driver<REQ>(name), vif(0) {}
    UVM_COMPONENT_PARAM_UTILS(vip_driver<REQ>);
    virtual void build_phase(uvm_phase &phase)
    {
        uvm_driver<REQ>::build_phase(phase);
        if (!uvm_config_db<vip_if *>::get(this, "*", "vif", vif))
            UVM_FATAL(this->get_name(), "Interface to DUT not defined! Simulation aborted!");
    }
    virtual void run_phase(uvm_phase &phase)
    {
        REQ req, rsp;
        while (true) // forever loop
        {
            this->seq_item_port->get_next_item(req);
            drive_transfer(req);
            rsp.set_id_info(req);
            this->seq_item_port->item_done();
            this->seq_item_port->put_response(rsp);
        }
    }
    virtual void drive_transfer(const REQ &p)
    {
        vif->sig_a.write(...);
    }
};

template <class REQ>
class new_driver : public vip_driver<REQ>
{
public:
    new_driver(uvm_name name) : vip_driver<REQ>(name) {}
    UVM_COMPONENT_PARAM_UTILS(new_driver<REQ>);
};

/*============MONITOR============*/

class vip_monitor : public uvm_monitor
{
public:
    uvm_analysis_port<vip_trans> item_collected_port;
    vip_if *vif;
    bool checks_enable;
    bool coverage_enable;
    vip_monitor(uvm_name name) : uvm_monitor(name),
                                 item_collected_port("item_collected_port"), vif(0),
                                 checks_enable(false), coverage_enable(false) {}
    UVM_COMPONENT_UTILS(vip_monitor);
    virtual void build_phase(uvm_phase &phase)
    {
        uvm_monitor::build_phase(phase);
        uvm_config_db<bool>::get(this, "*", "checks_enable", checks_enable);
        uvm_config_db<bool>::get(this, "*", "coverage_enable", coverage_enable);
        if (!uvm_config_db<vip_if *>::get(this, "*", "vif", vif))
            UVM_FATAL(get_name(), "Interface to DUT not defined! Simulation aborted!");
    }
    virtual void run_phase(uvm_phase &phase)
{
    vip_trans p;
    while (true) // forever loop
    {
        // Collect the data into transaction vip_trans
        item_collected_port.write(p);
        if (checks_enable)
        {
            if (p.data < 0) {
                UVM_ERROR(get_name(), "Negative value detected!");
            }
        }
        if (coverage_enable)
        {
            uvm_report_info("Coverage", "Covered address %h" + p.address, UVM_LOW);
        }
    }
}

};