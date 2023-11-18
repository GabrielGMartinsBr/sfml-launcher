@w = 640
@h = 480

def drawGrid
    spr = Sprite.new()
    bit = Bitmap.new(@w, @h)
    spr.bitmap = bit

    bg = Color.new(255, 255, 255)
    line = Color.new(205, 205, 205)
    size = 64

    for x in 0...@w
        for y in 0...@h
            if x % size == 0 || y % size == 0
                bit.set_pixel(x, y, line)
            else
                bit.set_pixel(x, y, bg)
            end
        end
    end
end

def drawSprTest(dimension)
    # vp = Viewport.new(0, 0, 640, 480)
    vp = Viewport.new(10, 10, 640, 480)
    spr = Sprite.new(vp)
    bit = Bitmap.new(dimension + 1, dimension + 1)
    spr.bitmap = bit

    bg = Color.new(255, 255, 255)
    bgAlt = Color.new(128, 128, 128)
    line = Color.new(105, 105, 255)
    size = 32

    spr.x  = 128
    spr.y  = 128

    for x in 0..dimension
        for y in 0..dimension
            if x % dimension % size == 0 || y % dimension % size == 0
                bit.set_pixel(x, y, line)
            elsif (x / 32).floor % 2 === (y / 32).floor % 2
                bit.set_pixel(x, y, bgAlt)
            else
                bit.set_pixel(x, y, bg)
            end
        end
    end

    #spr.ox = 16
    #spr.oy = 32
    
    #spr.src_rect = Rect.new(0, 0, dimension / 2, dimension / 2)
    # spr.src_rect = Rect.new(16, 31, dimension / 2, dimension / 2)


end

drawGrid()

drawSprTest(256)

loop do
    Graphics.update
end
