
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Linux kernel greeting logger");

struct greeting_info {
    struct list_head node;
    ktime_t time_start;
    ktime_t time_stop;
};

static LIST_HEAD(greeting_log);

void print_hello(void)
{
    struct greeting_info *info;

    info = kmalloc(sizeof(*info), GFP_KERNEL);
    if (!info) {
        pr_err("hello1: Allocation error\n");
        return;
    }

    info->time_start = ktime_get();
    pr_info("hello1: Hello from the kernel!\n");
    info->time_stop = ktime_get();

    list_add_tail(&info->node, &greeting_log);
}

EXPORT_SYMBOL(print_hello);

static int __init logger_init(void)
{
    pr_info("hello1: Logger initialized\n");
    return 0;
}

static void __exit logger_exit(void)
{
    struct greeting_info *info, *next;
    unsigned int counter = 0;

    list_for_each_entry_safe(info, next, &greeting_log, node) {
        s64 duration = ktime_to_ns(
            ktime_sub(info->time_stop, info->time_start));

        pr_info("hello1: Greeting %u took %lld ns\n",
                ++counter, (long long)duration);

        list_del(&info->node);
        kfree(info);
    }

    pr_info("hello1: Logger stopped\n");
}

module_init(logger_init);
module_exit(logger_exit);
