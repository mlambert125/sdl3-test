for y = 0, 499 do
    for x = 0, 499 do
        if math.fmod(x, 5) == 0 and math.fmod(y, 5) == 0 then
            setTile(y, x, 1)
        else
            setTile(y, x, 0)
        end
    end
end
setPlayerPosition(25, 25)
printLuaTable({ x = 25, y = 25 })
