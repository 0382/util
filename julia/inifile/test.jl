include("inifile.jl")

function main()
    ini = IniFile("test.ini")
    if !isgood(ini)
        error(error_msg(ini))
    end
    println(getString(ini, "name"))
    println(getInt(ini, "section1", "test"))
end

main()
