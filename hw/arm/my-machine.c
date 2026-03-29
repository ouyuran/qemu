#include "qemu/osdep.h"
#include "hw/core/boards.h"
#include "hw/arm/machines-qom.h"
#include "hw/arm/boot.h"
#include "hw/core/sysbus.h"
#include "system/address-spaces.h"
#include "qemu/error-report.h"
#include "qapi/error.h"
#include "target/arm/cpu-qom.h"

static struct arm_boot_info my_binfo;

static void my_machine_init(MachineState *machine)
{
    MemoryRegion *system_mem = get_system_memory();

    Object *cpuobj = object_new(machine->cpu_type);
    qdev_realize(DEVICE(cpuobj), NULL, &error_fatal);
    ARMCPU *cpu = ARM_CPU(cpuobj);

    // 👉 DDR（RAM）
    MemoryRegion *ram = g_new(MemoryRegion, 1);
    memory_region_init_ram(ram, NULL, "ram", 128 * 1024 * 1024, &error_fatal);
    memory_region_add_subregion(system_mem, 0x80000000, ram);

    // 👉 MYDEV 外设
    DeviceState *dev = qdev_new("mydev");
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, 0x40000000);

    my_binfo.ram_size = machine->ram_size;
    my_binfo.loader_start = 0x80000000;
    my_binfo.is_linux = 0;
    arm_load_kernel(cpu, machine, &my_binfo);
}

static void my_machine_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "My Minimal Machine";
    mc->init = my_machine_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-a7");
}

static const TypeInfo my_machine_type = {
    .name          = MACHINE_TYPE_NAME("my-machine"),
    .parent        = TYPE_MACHINE,
    .class_init    = my_machine_class_init,
    .interfaces    = arm_machine_interfaces,
};

static void my_machine_register_types(void)
{
    type_register_static(&my_machine_type);
}

type_init(my_machine_register_types);