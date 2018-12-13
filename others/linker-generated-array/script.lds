SECTIONS
{
  .rodata.array :
  {
    *(SORT_BY_NAME(.array*))
  }
}
INSERT AFTER .rodata;
