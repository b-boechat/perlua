-- Arquivo sample2.lua

function multiplicar_por_2000(n)
    dois_mil = 2e3
    for i = 1, 19 do
        b = "Esse loop só existe\nPara fins didáticos"
        len_b = #b
        print (len_b)
    end
    --[[ Comentário

    de múltiplas

    linhas ]]
    return dois_mil * n
end
