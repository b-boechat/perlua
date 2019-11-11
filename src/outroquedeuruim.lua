local previous, current = 0, 1
local it, i = 40, 1
while i <= it do
    if current % 2 == 0 then
        print (current, "EVEN!")
    else
        print (current, "ODD!");
    end
    current, previous, i = current + previous, current, i + 1
end
    
