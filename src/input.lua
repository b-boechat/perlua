x = 3
y = 7
print (x, y) -- 3   7
do
    local x = "oi"
    print (x) -- oi
    x, y = y, x
    print (x, y) -- 7   oi
end
print (x)   -- 3
