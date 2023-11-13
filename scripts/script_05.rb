$w = 640
$h = 480

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
        spr = Sprite.new
        spr.bitmap = bp
    end
end

bg = Background.new
char = Character.new

loop do
    Graphics.update
end