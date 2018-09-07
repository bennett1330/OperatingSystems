#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct list_head birthday_list;

struct birthday {
	int day, month, year;
	struct list_head list;
};

int add_person(struct list_head *birthday_list, int d, int m, int y) {
	struct birthday *person;
	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = d;
	person->month = m;
	person->year = y;
	list_add_tail(&person->list, birthday_list);
	return 0;
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
	struct birthday *curr;
	printk(KERN_INFO "Loading Module\n");
	INIT_LIST_HEAD(&birthday_list);
	add_person(&birthday_list, 2, 8, 1995);
	add_person(&birthday_list, 20, 6, 1855);
	add_person(&birthday_list, 12, 2, 2005);
	add_person(&birthday_list, 30, 9, 1990);
	add_person(&birthday_list, 8, 10, 1965);
	list_for_each_entry(curr, &birthday_list, list) {
		printk( KERN_INFO "%d-%d-%d", curr->day, curr->month, curr->year);
	}
	printk( KERN_INFO "End Load\n");
	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	struct birthday *curr, *next;
	printk(KERN_INFO "Removing Module\n");
	list_for_each_entry_safe(curr, next, &birthday_list, list) {
		printk( KERN_INFO "Removing %d-%d-%d", curr->day, curr->month, curr->year);
		list_del(&curr->list);
		kfree(curr);
	}
	printk( KERN_INFO "End remove\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

