# 要是想实际使用，还是去看看我的`CGcoefficient.jl`包吧
# 这个效率很低，而且精度也不高
# 只是推导了一大堆，还是记录一下吧
function S(a::Int, b::Int, c::Int, d::Int, e::Int, f::Int)
    C1 = div(-a + e + f, 2)
    C2 = div(d - b + f, 2)
    C3 = -f
    C4 = div(d + e + c, 2) + 1
    C5 = div(a + e - f, 2) + 1
    C6 = div(d + b - f, 2) + 1
    M = min(C1, C2)
    F = zeros(Float64, M + 1)
    n1 = div(-a + b + c, 2)
    n2 = div(a - b + c, 2)
    n3 = div(a + b - c, 2)
    A::Float64 = 0.0
    if C1 <= C2
        for x in 0:M
            y = M - x
            F[x+1] = binomial(n3, div(b - d + f, 2) - y)
            A = binomial(n2, div(-d + e + c, 2))
        end
    else
        for x in 0:M
            y = M - x
            F[x+1] = binomial(n3, div(a - e + f, 2) - y)
            A = binomial(n1, div(d - e + c, 2))
        end
    end
    for t = 1:M
        for x in 0:(M-t)
            y = M - t - x
            F[x+1] = div((C3 + x + 2y) * (C4 - x) * F[x+2] + (C5 + y) * (C6 + y) * F[x+1], (C1 - M + t) * (C2 - M + t))
        end
    end
    return F[1] * A * (-1)^div(a + b + c, 2)
end

function my6j(dj1::Int, dj2::Int, dj3::Int, dj4::Int, dj5::Int, dj6::Int)
    j123::Int = div(dj1 + dj2 + dj3, 2)
    j156::Int = div(dj1 + dj5 + dj6, 2)
    j426::Int = div(dj4 + dj2 + dj6, 2)
    j453::Int = div(dj4 + dj5 + dj3, 2)
    jpm123::Int = div(dj1 + dj2 - dj3, 2)
    jpm156::Int = div(dj1 + dj5 - dj6, 2)
    jpm426::Int = div(dj4 + dj2 - dj6, 2)
    jpm453::Int = div(dj4 + dj5 - dj3, 2)
    nA = binomial(j123 + 1, dj1 + 1) * binomial(dj1, jpm123)
    d1 = binomial(j156 + 1, dj1 + 1) * binomial(dj1, jpm156)
    d2 = binomial(j453 + 1, dj4 + 1) * binomial(dj4, jpm453)
    d3 = binomial(j426 + 1, dj4 + 1) * binomial(dj4, jpm426)
    A = sqrt(float(nA) / (float(d1) * float(d2) * float(d3)))
    B = S(dj1, dj2, dj3, dj4, dj5, dj6)
    return A * B / (dj4 + 1)
end
