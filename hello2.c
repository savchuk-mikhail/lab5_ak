
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Linux kernel greeting manager");

static unsigned int count = 1;

module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of greeting messages");

static int __init manager_init(void)
{
    unsigned int i;

    pr_info("hello2: Requested greetings: %u\n", count);

    if (count > 10) {
        pr_err("hello2: Invalid count, maximum is 10\n");
        return -EINVAL;
    }

    if (count == 0)
        pr_warn("hello2: No messages to print\n");
    else if (count >= 5)
        pr_warn("hello2: High number of greetings\n");

    for (i = 0; i < count; i++)
        print_hello();

    return 0;
}

static void __exit manager_exit(void)
{
    pr_info("hello2: Manager unloaded\n");
}

module_init(manager_init);
module_exit(manager_exit);
