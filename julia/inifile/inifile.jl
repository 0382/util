# @author:      0.382
# @description: ini文件读取
# 

mutable struct IniItem
    key::String
    value::Union{Bool,Int64,Float64,String,Nothing}
end

mutable struct IniSection
    name::String
    items::Vector{IniItem}
end

struct IniFile
    sections::Vector{IniSection}
    good::Bool
    err_msg::String
    IniFile(secs::Vector{IniSection}, good::Bool, msg::AbstractString) = new(secs, good, msg)
end

"if open ini file success"
isgood(ini::IniFile) = ini.good

"error message of IniFile"
error_msg(ini::IniFile) = ini.err_msg

IniFile(secs::Vector{IniSection}) = IniFile(secs, true, "")

"construct inifile from file"
IniFile(fname::AbstractString) = begin
    try
        secs = open(read_inifile, fname, "r")
        return IniFile(secs, true, "")
    catch err
        msg = ""
        if err isa SystemError
            msg = "cannot " * err.prefix
        else
            msg = string(err)
        end
        return IniFile(Vector{IniSection}(), false, msg)
    end
end

function read_inifile(fp::IO)::Vector{IniSection}
    inisections = Vector{IniSection}()
    inisec = IniSection("", Vector{IniItem}())
    iniitem = IniItem("", "")
    for line in eachline(fp)
        line = strip(remove_comment(line))
        line == "" && continue
        if line[1] == '[' && line[end] == ']'
            push!(inisections, deepcopy(inisec))
            inisec = IniSection(strip(line, ('[', ']', ' ')), Vector{IniItem}())
        elseif build_item!(line, iniitem)
            push!(inisec.items, deepcopy(iniitem))
        else
            throw("invalid ini line: " * line)
        end
    end
    push!(inisections, inisec)
end

function getValue(ini::IniFile, section::String, key::String)::Union{Bool,Int64,Float64,String}
    try
        err_secname = section == "" ? "(default)" : section
        pos = findfirst(x -> x.name == section, ini.sections)
        pos === nothing && throw(KeyError("no section '$err_secname'"))
        sec::IniSection = ini.sections[pos]
        pos = findfirst(x -> x.key == key, sec.items)
        pos === nothing && throw(KeyError("no key '$key' in section '$err_secname'"))
        return sec.items[pos].value
    catch err
        println("get failed: ", err)
        exit(-1)
    end
end

getString(ini::IniFile, section::String, key::String) = getValue(ini, section, key)::String
getBool(ini::IniFile, section::String, key::String) = getValue(ini, section, key)::Bool
getInt(ini::IniFile, section::String, key::String) = getValue(ini, section, key)::Int64
getFloat(ini::IniFile, section::String, key::String) = getValue(ini, section, key)::Float64

getValue(ini::IniFile, key::String) = getValue(ini, "", key)
getString(ini::IniFile, key::String) = getString(ini, "", key)
getBool(ini::IniFile, key::String) = getBool(ini, "", key)
getInt(ini::IniFile, key::String) = getInt(ini, "", key)
getFloat(ini::IniFile, key::String) = getFloat(ini, "", key)

Base.show(io::IO, ini::IniFile) = begin
    for sec in ini.sections
        sec.name == "" || println(io, "[$(sec.name)]")
        for it in sec.items
            println(io, "$(it.key) = $(it.value)")
        end
    end
end

function saveAs(ini::IniFile, fname::AbstractString)
    open(fname, "w") do io
        print(io, ini)
    end
end

# 去掉尾部的注释
function remove_comment(line::String)::String
    rl = line[:]
    for c in ["#", ";", "//"]
        pos = findfirst(c, rl)
        pos === nothing || (rl = rl[1:pos.start - 1])
    end
    return rl
end

# 利用一行字符串创建 IniItem
function build_item!(line::AbstractString, it::IniItem)
    pos = findfirst('=', line)
    pos === nothing && return false
    it.key = strip(line[1:pos - 1], [' ', '\'', '\"'])
    temp::String = strip(line[pos + 1:end], [' ', '\'', '\"'])
    it.value = tryparse(Bool, temp)
    it.value === nothing || return true
    it.value = tryparse(Int64, temp)
    it.value === nothing || return true
    it.value = tryparse(Float64, temp)
    it.value === nothing || return true
    it.value = temp
    it.value == "" || return true
    return false
end
