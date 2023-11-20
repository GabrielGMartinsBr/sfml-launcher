$w = 640
$h = 480

def drawGrid
    spr = Sprite.new()
    bit = Bitmap.new($w, $h)
    spr.bitmap = bit

    bg = Color.new(64, 64, 64)
    centerLine = Color.new(255, 172, 172)
    line = Color.new(205, 205, 205)
    size = 32

    for x in 0...$w
        for y in 0...$h
            if x == $w / 2 || y == $h / 2
                bit.set_pixel(x, y, centerLine)
            elsif x % size == 0 || y % size == 0
                bit.set_pixel(x, y, line)
            else
                bit.set_pixel(x, y, bg)
            end
        end
    end
end

class Background
    def initialize
        spr = Sprite.new
        spr.bitmap = Bitmap.new($w, $h)
        
        color = Color.new(0, 0, 0)
        
        for x in 0...$w
            for y in 0...$h
                color.set(x, y, 0);
                spr.bitmap.set_pixel(x, y, color)
            end
        end
    end
end

class Character
    def initialize
        bp = Bitmap.new("Graphics/Characters/001-Fighter01")
        
        col = bp.width / 4
        row = bp.height / 4
        
        # vp = Viewport.new(0, 0, col, row)
        rect = Rect.new(64, 64, col, row)
        vp = Viewport.new(rect)

        vp.rect.x = 32
        vp.rect.y = 32

        vp.ox = 32

        spr = Sprite.new(vp)
        # spr = Sprite.new()
        spr.bitmap = bp

        spr.x = 0
        spr.y = 0
        spr.opacity = 255
        
        # spr.ox = 32
        # spr.oy = 0

        # puts "z: " + spr.z.to_s
        # spr.z = 3
        # puts "z: " + spr.z.to_s
        # spr.z = 9
        # puts "z: " + spr.z.to_s
        
        # puts "\n"
        # puts "---Rect--------------"
        # puts "x: " + vp.rect.x.to_s
        # puts "y: " + vp.rect.y.to_s
        # puts "width: " + vp.rect.width.to_s
        # puts "height: " + vp.rect.height.to_s
        # puts "---------------------"
        # puts "\n"

        

    end
end

def drawRect
    size = 200
    c1 = Color.new(100, 100, 100)

    spr = Sprite.new
    spr.x = 100
    spr.y = 100

    rect = Rect.new(0, 0, size, size)
    spr.bitmap = Bitmap.new(640, 480)

    # spr.bitmap.fill_rect(0, 0, size, size, Color.new(255, 255, 255))
    # spr.bitmap.fill_rect(rect, Color.new(0, 155, 255))
    spr.bitmap.fill_rect(rect, Color.new(255, 255, 255))
end



# bg = Background.new
# drawRect
drawGrid
char = Character.new

loop do
    Graphics.update
end