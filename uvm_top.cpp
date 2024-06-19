#include <uvm>
#include <systemc>

#include "process.h"
#include "uvm_agent.h"
#include "uvm_driver_monitor.h"
#include "uvm_scoreboard.h"
#include "uvm_sequence.h"
#include "uvm_test.h"
#include <iostream>
#include <vector>
#include <string>

using namespace uvm;

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