#include "qemu/osdep.h"
#include "hw/core/sysbus.h"
#include "qemu/module.h"

#define TYPE_MYDEV "mydev"
OBJECT_DECLARE_SIMPLE_TYPE(MyDevState, MYDEV)

struct MyDevState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;

    uint32_t reg_data;
};

static uint64_t mydev_read(void *opaque, hwaddr addr, unsigned size)
{
    MyDevState *s = opaque;

    switch (addr) {
    case 0x0:
        return s->reg_data;
    default:
        return 0;
    }
}

static void mydev_write(void *opaque, hwaddr addr,
                        uint64_t val, unsigned size)
{
    MyDevState *s = opaque;

    switch (addr) {
    case 0x0:
        s->reg_data = val;

        // 👉 模拟硬件行为
        fprintf(stderr, "MYDEV: write %lu\n", val);
        break;
    }
}

static const MemoryRegionOps mydev_ops = {
    .read = mydev_read,
    .write = mydev_write,
};

static void mydev_init(Object *obj)
{
    MyDevState *s = MYDEV(obj);

    memory_region_init_io(&s->iomem, obj, &mydev_ops, s,
                          "mydev-mmio", 0x1000);
}

static void mydev_realize(DeviceState *dev, Error **errp)
{
    MyDevState *s = MYDEV(dev);

    sysbus_init_mmio(SYS_BUS_DEVICE(dev), &s->iomem);
}

static void mydev_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize = mydev_realize;
}

static const TypeInfo mydev_info = {
    .name          = TYPE_MYDEV,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MyDevState),
    .instance_init = mydev_init,
    .class_init    = mydev_class_init,
};

static void mydev_register_types(void)
{
    type_register_static(&mydev_info);
}

type_init(mydev_register_types);