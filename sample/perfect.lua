num, max_num = 1, 200
while (num <= max_num) do
    i, soma = 1, 0
    while (i < num) do
        if num % i == 0 then
            soma = soma + i
        end
        i = i + 1
    end
    if (soma == num) then
        print (num, "Número perfeito!")
    end
    num = num + 1
end


