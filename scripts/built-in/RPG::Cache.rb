module RPG
  module Cache
    @cache = {}
    def self.load_bitmap(folder_name, filename, hue = 0)
      path = folder_name + filename
      if !@cache.include?(path) or @cache[path].disposed?
        @cache[path] = if filename != ''
                         Bitmap.new(path)
                       else
                         Bitmap.new(32, 32)
                       end
      end
      if hue == 0
        @cache[path]
      else
        key = [path, hue]
        if !@cache.include?(key) or @cache[key].disposed?
          @cache[key] = @cache[path].clone
          @cache[key].hue_change(hue)
        end
        @cache[key]
      end
    end

    def self.animation(filename, hue)
      load_bitmap('Graphics/Animations/', filename, hue)
    end

    def self.autotile(filename)
      load_bitmap('Graphics/Autotiles/', filename)
    end

    def self.battleback(filename)
      load_bitmap('Graphics/Battlebacks/', filename)
    end

    def self.battler(filename, hue)
      load_bitmap('Graphics/Battlers/', filename, hue)
    end

    def self.character(filename, hue)
      load_bitmap('Graphics/Characters/', filename, hue)
    end

    def self.fog(filename, hue)
      load_bitmap('Graphics/Fogs/', filename, hue)
    end

    def self.gameover(filename)
      load_bitmap('Graphics/Gameovers/', filename)
    end

    def self.icon(filename)
      load_bitmap('Graphics/Icons/', filename)
    end

    def self.panorama(filename, hue)
      load_bitmap('Graphics/Panoramas/', filename, hue)
    end

    def self.picture(filename)
      load_bitmap('Graphics/Pictures/', filename)
    end

    def self.tileset(filename)
      load_bitmap('Graphics/Tilesets/', filename)
    end

    def self.title(filename)
      load_bitmap('Graphics/Titles/', filename)
    end

    def self.windowskin(filename)
      load_bitmap('Graphics/Windowskins/', filename)
    end

    def self.tile(filename, tile_id, hue)
      key = [filename, tile_id, hue]
      if !@cache.include?(key) or @cache[key].disposed?
        @cache[key] = Bitmap.new(32, 32)
        x = (tile_id - 384) % 8 * 32
        y = (tile_id - 384) / 8 * 32
        rect = Rect.new(x, y, 32, 32)
        @cache[key].blt(0, 0, tileset(filename), rect)
        @cache[key].hue_change(hue)
      end
      @cache[key]
    end

    def self.clear
      @cache = {}
      GC.start
    end
  end
end
