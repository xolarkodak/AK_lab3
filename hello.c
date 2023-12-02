// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("IO-11 Kudelia Sasha");
MODULE_DESCRIPTION("Simple chrono counter for my lab");
MODULE_LICENSE("Dual BSD/GPL");

static uint chrono_counter = 1;

module_param(chrono_counter, uint, 0444);
MODULE_PARM_DESC(chrono_counter, "my counter");

struct my_struct {
	struct list_head tlist;
	ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	int i;

	if (chrono_counter == 0) {
		printk(KERN_WARNING "Success! The parameter's 0\n");
	} else if (chrono_counter >= 5 && chrono_counter <= 10) {
		printk(KERN_WARNING "Success! The parameter's between 5 and 10\n");
	} else if (chrono_counter > 10) {
		printk(KERN_WARNING "Fail! The parameter's > 10)\n");
		return -EINVAL;
	}

	for (i = 0; i < chrono_counter; i++) {
		struct my_struct *ms = NULL;

		ms = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
		ms->time = ktime_get();
		list_add_tail(&ms->tlist, &my_list_head);

		printk(KERN_INFO "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	int count = 0;
	struct my_struct *t, *tmp;

	list_for_each_entry_safe(t, tmp, &my_list_head, tlist) {
		pr_info("i: %d; ktime = %lld\n", count++, ktime_to_ns(t->time));
		list_del(&t->tlist);
		kfree(t);
	}
	printk(KERN_INFO "Success! Module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
