using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LunarGridMesh {
    class Program {
        public const double LUNAR_RADIUS = 1737.4;
        public const double ANGLE2RAD = Math.PI / 180.0;
        public const double HALF_PI = 0.5 * Math.PI;
        public const double TWO_PI = 2.0 * Math.PI;
        public const double R_TWOPI = 1.0 / TWO_PI;

        static void Main(string[] args) {
            var inputs = new List<Record>();
            var rowSet = new HashSet<double>();
            var colSet = new HashSet<double>();
            string line;
            while ((line = Console.ReadLine()) != null) {
                try {
                    var r = Record.Parse(line);
                    inputs.Add(r);
                    rowSet.Add(r.vertical);
                    colSet.Add(r.horizontal);
                } catch { }
            }

            var nCols = colSet.Count;
            var nRows = rowSet.Count;
            var records = new List<Record>();
            for (var x = 0; x < nCols; x++)
                records.Add(new Record(inputs[x].horizontal, HALF_PI, 0.0));
            records.Add(new Record(inputs[0].horizontal + TWO_PI, HALF_PI, 0.0));
            for (var y = 0; y < nRows; y++) {
                for (var x = 0; x < nCols; x++)
                    records.Add(inputs[x + y * nCols]);
                var r = inputs[y * nCols];
                records.Add(new Record(r.horizontal + TWO_PI, r.vertical, r.height));
            }
            for (var x = 0; x < nCols; x++)
                records.Add(new Record(inputs[x].horizontal, -HALF_PI, 0.0));
            records.Add(new Record(inputs[0].horizontal + TWO_PI, -HALF_PI, 0.0));

            nCols += 1;
            nRows += 2;
            var vertices = new List<Vector3>();
            var uvs = new List<Vector2>();
            for (var y = 0; y < nRows; y++) {
                for (var x = 0; x < nCols; x++) {
                    var iRecord = x + y * nCols;
                    var r = records[iRecord];
                    var phi = r.vertical;
                    var theta = r.horizontal;
                    var cosphi = Math.Cos(phi);
                    var height = (r.height + LUNAR_RADIUS) / LUNAR_RADIUS;
                    vertices.Add(new Vector3(
                        height * cosphi * Math.Sin(theta), 
                        height * Math.Sin(phi),
                        height * cosphi * Math.Cos(theta)));
                    uvs.Add(new Vector2(theta * R_TWOPI, Clamp01((phi + HALF_PI) / Math.PI)));
                }
            }

            var triangles = new List<int>();
            for (var y = 0; y < (nRows - 1); y++) {
                for (var x = 0; x < (nCols - 1); x++) {
                    var iRecord = x + y * nCols + 1;
                    triangles.Add(iRecord);
                    triangles.Add(iRecord + nCols);
                    triangles.Add(iRecord + nCols + 1);
                    triangles.Add(iRecord);
                    triangles.Add(iRecord + nCols + 1);
                    triangles.Add(iRecord + 1);
                }
            }

            Console.WriteLine("g Lunar");
            foreach (var v in vertices)
                Console.WriteLine("v {0:e} {1:e} {2:e}", v.x, v.y, v.z);
            foreach (var uv in uvs)
                Console.WriteLine("vt {0:e} {1:e}", uv.x, uv.y);
            for (var i = 0; i < triangles.Count; i += 3)
                Console.WriteLine("f {0} {1} {2}", triangles[i], triangles[i + 1], triangles[i + 2]);
            
        }
        public static double Clamp01(double v) {
            return v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v);
        }

        public struct Record {
            public double horizontal;
            public double vertical;
            public double height;

            public Record(double horizontal, double vertical, double height) {
                this.horizontal = horizontal;
                this.vertical = vertical;
                this.height = height;
            }

            public static Record Parse(string line) {
                var cols = line.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                return new Record(
                    double.Parse(cols[0]) * ANGLE2RAD,
                    double.Parse(cols[1]) * ANGLE2RAD,
                    double.Parse(cols[2]));
            }
        }

        public struct Vector3 {
            public double x;
            public double y;
            public double z;

            public Vector3(double x, double y, double z) {
                this.x = x;
                this.y = y;
                this.z = z;
            }
        }
        public struct Vector2 {
            public double x;
            public double y;

            public Vector2(double x, double y) {
                this.x = x;
                this.y = y;
            }
        }
    }
}
