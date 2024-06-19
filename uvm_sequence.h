#include <uvm>
#include <systemc>

#include "process.h"
#include <iostream>
#include <vector>
#include <string>

using namespace uvm;

/*============SEQUENCER============*/
template <class REQ>
class vip_sequencer : public uvm_sequencer<REQ>
{
public:
    vip_sequencer(uvm_name name)
        : uvm_sequencer<REQ>(name) {}
    UVM_COMPONENT_PARAM_UTILS(vip_sequencer<REQ>);
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
    //virtual void do_print(uvm_printer &printer) const { ... }
    //virtual void do_pack(uvm_packer &packer) const { ... }
    //virtual void do_unpack(uvm_packer &packer) { ... }
    //virtual void do_copy(const uvm_object *rhs) { ... }
    //virtual bool do_compare(const uvm_object *rhs) const { ... }
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
        start_item(req);
        // req->randomize(); // doesn't work
        finish_item(req);
        get_response(rsp);
    }
    virtual void post_body()
    {
        if (starting_phase != NULL)
            starting_phase->drop_objection(this);
    }
};