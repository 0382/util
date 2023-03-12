module console_m
    use iso_c_binding
    implicit none
    type console_size
        integer(c_int) :: rows
        integer(c_int) :: cols
    end type

    interface
        integer(c_int64_t) function get_console_size() bind(c)
            import :: c_int64_t
        end function
    end interface
contains

end module console_m