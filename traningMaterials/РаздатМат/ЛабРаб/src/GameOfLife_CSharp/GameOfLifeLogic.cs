using System;
using System.Drawing;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using Microsoft.Drawing;

namespace GameOfLife
{
    internal class GameBoard
    {
        private Color?[][,] _scratch;
        private int _currentIndex;
        private ObjectPool<Bitmap> _pool;

        public GameBoard(int width, int height, double initialDensity, ObjectPool<Bitmap> pool)
        {
            // Validate parameters
            if (width < 1) throw new ArgumentOutOfRangeException("width");
            if (height < 1) throw new ArgumentOutOfRangeException("height");
            if (pool == null) throw new ArgumentNullException("pool");
            if (initialDensity < 0 || initialDensity > 1) throw new ArgumentOutOfRangeException("initialDensity");

            // Store parameters
            _pool = pool;
            Width = width;
            Height = height;

            // Create the storage arrays
            _scratch = new Color?[2][,] { new Color?[width, height], new Color?[width, height] };

            // Populate the board randomly based on the provided initial density
            Random rand = new Random();
            for (int i = 0; i < width; i ++)
            {
                for (int j = 0; j < height; j++)
                {
                    _scratch[_currentIndex][i, j] = (rand.NextDouble() < initialDensity) ? Color.FromArgb(rand.Next()) : (Color?)null;
                }
            }
        }

    public Bitmap MoveNext()
        {
            int nextIndex = (_currentIndex + 1) % 2;
            Color?[,] current = _scratch[_currentIndex];
            Color?[,] next = _scratch[nextIndex];
            Random rand = new Random();

            var bmp = _pool.GetObject();
            using (FastBitmap fastBmp = new FastBitmap(bmp))
            {
                // For every row
                Action<int> body = i =>
                {
                    // For every column
                    for (int j = 0; j < Height; j++)
                    {
                        int count = 0;
                        int r = 0, g = 0, b = 0;

                        // Count neighbors
                        for (int x = i - 1; x <= i + 1; x++)
                        {
                            for (int y = j - 1; y <= j + 1; y++)
                            {
                                if ((x == i && j == y) || x < 0 || x >= Width || y < 0 || y >= Height) continue;
                                Color? c = current[x, y];
                                if (c.HasValue)
                                {
                                    count++;
                                    r += c.Value.R;
                                    g += c.Value.G;
                                    b += c.Value.B;
                                }
                            }
                        }

                        if (count < 1 || count >= 4) next[i, j] = null;
                        else if (current[i, j].HasValue && (count == 2 || count == 3)) next[i, j] = current[i, j];
                        else if (!current[i, j].HasValue && count == 3) next[i, j] = Color.FromArgb(r / count, g / count, b / count);
                        else next[i, j] = null;

                        fastBmp.SetColor(i, j, current[i, j] ?? Color.White);
                    }
                };

                if (RunParallel)
                {
                    Parallel.For(0, Width, 
                        body);
                }
                else
                {
                    for (int i = 0; i < Width; i++) 
                        body(i);
                }
            }

            _currentIndex = nextIndex;
            return bmp;
        }

        public int Width { get; private set; }
        public int Height { get; private set; }
        public bool RunParallel { get; set; }
    }
}