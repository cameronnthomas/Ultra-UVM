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

/*============SEQUENCER============*/
template <class REQ>
class vip_sequencer : public uvm_sequencer<REQ>
{
public:
    vip_sequencer(uvm_name name)
        : uvm_sequencer<REQ>(name) {}
    UVM_COMPONENT_PARAM_UTILS(vip_sequencer<REQ>);
};

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
        ... vif->sig_a.write(...);
    }
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
            ... // Collect the data into transaction vip_trans
                item_collected_port.write(p);
            if (checks_enable)
            {
                ...
            }
            if (coverage_enable)
            {
                ...
            }
        }
    }
};

/*============VIP TRANS============*/
class vip_trans : public uvm_sequence_item
{
public:
    int addr;
    int data;
    bus_op_t op;
    vip_trans(const std::string &name = "vip_trans")
        : addr(0x0), data(0x0), op(BUS_READ) {}
    UVM_OBJECT_UTILS(vip_trans);
    virtual void do_print(uvm_printer &printer) const { ... }
    virtual void do_pack(uvm_packer &packer) const { ... }
    virtual void do_unpack(uvm_packer &packer) { ... }
    virtual void do_copy(const uvm_object *rhs) { ... }
    virtual bool do_compare(const uvm_object *rhs) const { ... }
};

/*============VIP SEQUENCE============*/

template <typename REQ = uvm_sequence_item, typename RSP = REQ>
class vip_sequence : public uvm_sequence<REQ, RSP>
{
public:
    vip_sequence(const std::string &name)
        : uvm_sequence<REQ, RSP>(name) {}
    UVM_OBJECT_PARAM_UTILS(vip_sequence<REQ, RSP>);
    virtual void pre_body()
    {
        if (starting_phase != NULL)
            starting_phase->raise_objection(this);
    }
    virtual void body()
    {
        REQ *req = new REQ();
        RSP *rsp = new RSP();
        ... start_item(req);
        // req->randomize(); // randomization compatibility layer not yet available in UVM-SystemC
        finish_item(req);
        get_response(rsp);
    }
    virtual void post_body()
    {
        if (starting_phase != NULL)
            starting_phase->drop_objection(this);
    }
};

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
        //...
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

template <class REQ>
class new_driver : public vip_driver<REQ>
{
public:
    new_driver(uvm_name name) : vip_driver<REQ>(name) {}
    UVM_COMPONENT_PARAM_UTILS(new_driver<REQ>);
    ...
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

/*============TOP LEVEL============*/

int sc_main(int, char *[])
{
    dut *my_dut = new dut("my_dut");
    vip_if *vif_uvc1 = new vip_if;
    vip_if *vif_uvc2 = new vip_if;
    uvm_config_db<vip_if *>::set(0, "*.uvc1.*",
                                 "vif", vif_uvc1);
    uvm_config_db<vip_if *>::set(0, "*.uvc2.*",
                                 "vif", vif_uvc2);
    my_dut->in(vif_uvc1->sig_a);
    my_dut->out(vif_uvc2->sig_a);
    run_test("test");
    sc_start();
    return 0;
}