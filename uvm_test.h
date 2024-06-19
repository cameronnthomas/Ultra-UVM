#include <uvm>
#include <systemc>

#include "process.h"
#include <iostream>
#include <vector>
#include <string>

using namespace uvm;

/*============TESTBENCH============*/

class testbench : public uvm_env
{
public:
    vip_uvc *uvc1;
    vip_uvc *uvc2;
    scoreboard *scoreboard1;
    testbench(uvm_name name) : uvm_env(name), uvc1(0),
                               uvc2(0), scoreboard1(0) {}
    UVM_COMPONENT_UTILS(testbench);
    virtual void build_phase(uvm_phase &phase)
    {
        uvm_env::build_phase(phase);
        uvc1 = vip_uvc::type_id::create("uvc1", this);
        assert(uvc1);
        uvc2 = vip_uvc::type_id::create("uvc2", this);
        assert(uvc2);
        scoreboard1 = scoreboard::type_id::create("scoreboard1", this);
        assert(scoreboard1);
        set_config_int("uvc1.*", "is_active", UVM_ACTIVE);
        set_config_int("uvc2.*", "is_active", UVM_PASSIVE);
    }
    virtual void connect_phase(uvm_phase &phase)
    {
        uvc1->agent->monitor->item_collected_port.connect(scoreboard1->xmt_listener_imp);
        uvc2->agent->monitor->item_collected_port.connect(scoreboard1->rcv_listener_imp);
    }
};

/*============TEST============*/

class test : public uvm_test
{
public:
    testbench *tb;
    bool test_pass;
    test(uvm_name name) : uvm_test(name),
                          tb(0), test_pass(true) {}
    UVM_COMPONENT_UTILS(test);
    virtual void build_phase(uvm_phase &phase)
    {
        uvm_test::build_phase(phase);
        tb = testbench::type_id::create("tb", this);
        assert(tb);
        uvm_config_db<uvm_object_wrapper *>::set(this,
                                                 tb.uvc1.agent.sequencer.run_phase, " default_sequence ",
                                                 vip_sequence<vip_trans>::type_id::get());
        set_type_override_by_type(vip_driver<vip_trans>::get_type(),
                                  new_driver<vip_trans>::get_type());
    }
    virtual void run_phase(uvm_phase &phase)
    {
        UVM_INFO(get_name(), "** UVM TEST STARTED **", UVM_NONE);
    }
    virtual void extract_phase(uvm_phase &phase)
    {
        if (tb->scoreboard1.error)
            test_pass = false;
    }
    virtual void report_phase(uvm_phase &phase)
    {
        if (test_pass)
            UVM_INFO(get_name(), "** UVM TEST PASSED **", UVM_NONE);
        else
            UVM_ERROR(get_name(), "** UVM TEST FAILED **");
    }
};

