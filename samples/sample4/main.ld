ENTRY(_start)

SECTIONS
{

	. = 0x8000;

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
