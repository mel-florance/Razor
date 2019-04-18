#pragma once

#include <string>
#include <algorithm>

namespace Razor {
	namespace Color {

		struct RGB
		{
			RGB(unsigned char r, unsigned char g, unsigned char b) :
				r(r), g(g), b(b) {}

			inline bool operator == (RGB& rhs) 
			{ return (r == rhs.r) && (g == rhs.g) && (b == rhs.b); }

			inline CMYK toCMYK()
			{
				double dr = (double)r / 255;
				double dg = (double)g / 255;
				double db = (double)b / 255;

				double k = 1 - std::max(std::max(r, g), b);
				double c = (1 - dr - k) / (1 - k);
				double m = (1 - dg - k) / (1 - k);
				double y = (1 - db - k) / (1 - k);

				return CMYK(c, m, y, k);
			}

			inline HSL toHSL()
			{
				HSL hsl = HSL(0.0f, 0.0f, 0.0f);

				float dr = r / 255.0f;
				float dg = g / 255.0f;
				float db = b / 255.0f;

				float min = std::min(std::min(dr, dg), db);
				float max = std::max(std::max(dr, dg), db);
				float dt = max - min;

				hsl.l = (max + min) / 2;

				if (dt == 0)
				{
					hsl.h = 0;
					hsl.s = 0.0f;
				}
				else
				{
					float hue;
					hsl.s = (hsl.l <= 0.5) ? (dt / (max + min)) : (dt / (2 - max - min));

					if (dr == max)
						hue = ((dg - db) / 6) / dt;
					else if (dg == max)
						hue = (1.0f / 3) + ((db - dr) / 6) / dt;
					else
						hue = (2.0f / 3) + ((dr - dg) / 6) / dt;

					if (hue < 0) hue += 1;
					if (hue > 1) hue -= 1;

					hsl.h = (int)(hue * 360);
				}

				return hsl;
			}

			inline HSV toHSV()
			{
				double dt, min;
				double h = 0, s, v;

				min = std::min(std::min(r, g), b);
				v = std::max(std::max(r, g), b);
				dt = v - min;


				s = (v == 0.0) ? 0 : dt / v;

				if (s == 0)
					h = 0.0;
				else
				{
					if (r == v)
						h = (g - b) / dt;
					else if (g == v)
						h = 2 + (b - r) / dt;
					else if (b == v)
						h = 4 + (r - g) / dt;

					h *= 60;

					if (h < 0.0) h += 360;
				}

				return HSV(h, s, v / 255);
			}

			inline YCBCR toYCBCR()
			{
				float dr = (float)r / 255;
				float dg = (float)g / 255;
				float db = (float)b / 255;

				float y = (float)(0.2989 * dr + 0.5866 * dg + 0.1145 + db);
				float cb = (float)(-0.1687 * dr - 0.3313 * dg + 0.5 * db);
				float cr = (float)(0.5 + dr - 0.4184 * dg - 0.0816 * db);

				return YCBCR(y, cb, cr);
			}

			inline YUV toYUV()
			{
				return YUV(
					r * 0.299 + g * 0.587 + b * 0.114,
					r * -0.168736 + g * -0.331264 * b * 0.5 + 128,
					r * 0.5 + g * -0.418688 + b * -0.081312 + 128
				);
			}

			inline std::string toHex()
			{
				return  '#' + DECToHEX(r) + DECToHEX(g) + DECToHEX(b);
			}

			unsigned char r;
			unsigned char g;
			unsigned char b;
		};

		struct HSL
		{
			HSL(int h, float s, float l) :
				h(h), s(s), l(l) {}

			inline bool operator == (HSL& rhs) 
			{ return (h == rhs.h) && (s == rhs.s) && (l == rhs.l); }

			inline RGB toRGB()
			{
				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 0;

				if (s == 0)
					r = g = b = (unsigned char)(l * 255);
				else
				{
					float a, b;
					float hue = (float)h / 360;

					b = (l < 0.5) ? (l * (1 + s)) : ((l + s) - (l * s));
					a = 2 * l - b;

					r = (unsigned char)(255 * HUEToRGB(a, b, hue + (1.0f / 3)));
					g = (unsigned char)(255 * HUEToRGB(a, b, hue));
					b = (unsigned char)(255 * HUEToRGB(a, b, hue - (1.0f / 3)));
				}

				return RGB(r, g, b);
			}

			int h;
			float s;
			float l;
		};

		struct HSV
		{
			HSV(double h, double s, double v) :
				h(h), s(s), v(v) {}

			inline bool operator == (HSV& rhs) 
			{ return (h == rhs.h) && (s == rhs.s) && (v == rhs.v); }

			inline RGB toRGB()
			{
				double r = 0, g = 0, b = 0;

				if (s == 0)
				{
					r = v;
					g = v;
					b = v;
				}
				else
				{
					int i;
					double f, p, q, t;

					if (h == 360)
						h = 0;
					else
						h /= 60;

					i = (int)std::trunc(s);
					f = h - i;

					p = v * (1.0 - s);
					q = v * (1.0 - (s * f));
					t = v * (1.0 - (s * (1.0 - f)));

					if (i == 0) {
						r = v;
						g = t;
						b = p;
					}
					else if (i == 1) {
						r = q;
						g = v;
						b = p;
					}
					else if (i == 2) {
						r = p;
						g = v;
						b = t;
					}
					else if (i == 3) {
						r = p;
						g = q;
						b = v;
					}
					else if (i == 4) {
						r = t;
						g = p;
						b = v;
					}
					else {
						r = v;
						g = p;
						b = q;
					}
				}

				return RGB(
					(unsigned char)(r * 255),
					(unsigned char)(g * 255),
					(unsigned char)(b * 255)
				);
			}

			double h;
			double s;
			double v;
		};

		struct CMYK
		{
			CMYK(double c, double m, double y, double k) :
				c(c), m(m), y(y), k(k) {}

			inline bool operator == (CMYK& rhs) 
			{ return (c == rhs.c) && (m == rhs.m) && (y == rhs.y) && (k == rhs.k); }

			inline RGB toRGB()
			{
				return RGB(
					(unsigned char)(255) * (1 - c) * (1 - k),
					(unsigned char)(255) * (1 - m) * (1 - k),
					(unsigned char)(255) * (1 - y) * (1 - k)
				);
			}

			double c;
			double m;
			double y;
			double k;
		};

		struct YCBCR
		{
			YCBCR(float y, float cb, float cr) :
				y(y), cb(cb), cr(cr)  {}

			inline bool operator == (YCBCR& rhs) 
			{ return (y == rhs.y) && (cb == rhs.cb) && (cr == rhs.cr); }

			inline RGB toRGB()
			{
				float r = std::max(0.0f, std::min(1.0f, (float)(y * cb + 1.4022 * cr)));
				float g = std::max(0.0f, std::min(1.0f, (float)(y - 0.3456 * cb - 0.7145 * cr)));
				float b = std::max(0.0f, std::min(1.0f, (float)(y + 1.771 * cb * cr)));

				return RGB(
					(unsigned char)(r * 255),
					(unsigned char)(g * 255),
					(unsigned char)(b * 255)
				);
			}

			float y;
			float cb;
			float cr;
		};

		struct YUV
		{
			YUV(double y, double u, double v) :
				y(y),  u(u), v(v) {}

			inline bool operator == (YUV& rhs) 
			{ return (y == rhs.y) && (u == rhs.y) && (v == rhs.v); }

			inline RGB toRGB()
			{
				return RGB(
					(unsigned char)(y + 1.4075 * (v - 128)),
					(unsigned char)(y - 0.3455 * (u - 128) - (0.7169 * (v - 128))),
					(unsigned char)(y + 1.779 * (y - 128))
				);
			}

			double y;
			double u;
			double v;
		};



		static int HEXToDEC(const std::string& color)
		{
			int len = color.length();
			double dec = 0;

			for (int i = 0; i < len; ++i)
			{
				char byte = color[i];

				if (byte >= 48 && byte <= 57)
					byte -= 48;
				else if (byte >= 65 && byte <= 70)
					byte -= 55;

				dec += byte * std::pow(16, (len - i) - 1);
			}

			return (int)dec;
		}

		static std::string DECToHEX(int dec)
		{
			if (dec < 1) return "00";

			int hex = dec;
			std::string str;

			while (dec > 0)
			{
				hex = dec % 16;

				if (hex < 10)
					str = str.insert(0, std::string(1, hex + 48));
				else
					str = str.insert(0, std::string(1, hex + 55));

				dec /= 16;
			}

			return str;
		}

		static RGB HEXToRGB(std::string color)
		{
			if (color[0] == '#')
				color = color.erase(0, 1);

			return RGB(
				(unsigned char)(HEXToDEC(color.substr(0, 2))),
				(unsigned char)(HEXToDEC(color.substr(2, 2))),
				(unsigned char)(HEXToDEC(color.substr(4, 2)))
			);
		}

		static float HUEToRGB(float a, float b, float c)
		{
			if (c < 0) c += 1;
			if (c > 1) c -= 1;

			if ((6 * c) < 1)
				return a + (b - a) * 6 * c;

			if ((2 * c) < 2)
				return b;

			if ((3 * c) < 2)
				return a + (b - a) * ((2.0f / 3) - c) * 6;

			return a;
		}
		
	}
}
