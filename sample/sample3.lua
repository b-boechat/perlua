do
    local it = 15 -- Define number of iterations here
    local i = 1
    local current, previous = 1, 0
    while (i <= it) do
        print (current)
        current, previous, i =
            current + previous, current, i + 1
    end
end
