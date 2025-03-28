local mapWidth = 40
local mapHeight = 40
local playerX = 15
local playerY = 15

initMap(mapWidth, mapHeight)
for y = 0, mapHeight - 1 do
    for x = 0, mapWidth - 1 do
        if math.fmod(x, 5) == 0 and math.fmod(y, 5) == 0 then
            setTile(y, x, 1)
        else
            setTile(y, x, 0)
        end
    end
end
setPlayerPosition(playerX, playerY)
printLuaTable({ x = 25, y = 25, z = { { 1, 2, 3 }, { 4, 5, 6 } } })
