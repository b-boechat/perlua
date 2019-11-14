do
    local previous, current = 0, 1
    local i, it = 0, 50
    while i <= it do
        i = i + 1
        if current % 2 == 0 then
            print (current, "EVEN!")
        else
            print (current, "ODD!")
        end
        previous, current = current, previous + current
    end
end
