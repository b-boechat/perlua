do
    local it = 50 -- Define number of iterations here
    local i = 0
    local current, previous = 1, 0
    while (i <= it) do
        print (current)
        current, previous, i =
            current + previous, current, i + 1
    end
end
