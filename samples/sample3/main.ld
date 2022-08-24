ENTRY(_start)

SECTIONS
{

	. = 0x0;

	/DISCARD/ :
	{
		*(.rela.plt*)
		*(.rela.dyn*)
		*(.comment*)
	}

	.text :
	{

		*(.text*)
		*(.*data*)
		*(.bss*)
	}

	.shstrtab : { *(.shstrtab) }

}
