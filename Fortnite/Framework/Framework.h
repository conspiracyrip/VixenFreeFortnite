#pragma once

namespace Framework
{
	namespace ColorPicker
	{
		float ColorAddWhite(float Current, float White)
		{
			Current += White;

			if (Current >= 255.f) return 255.f;

			return Current;
		}
	}

	class GUI
	{
	public:
		bool CheckHovered(FVector2D pos, FVector2D Size)
		{
			if (this->Cursor.X > pos.X && this->Cursor.Y > pos.Y)
				if (this->Cursor.X < pos.X + Size.X && this->Cursor.Y < pos.Y + Size.Y)
					return true;

			return false;
		}

		void ResetY()
		{
			this->Offset.Y = 0.0;
		}

		void PushX(double X)
		{
			this->Offset.X += X;
		}

		void PushY(double Y)
		{
			this->Offset.Y += Y;
		}
		void Combobox(FString Content, int* Value, FString Arguments[], int NumArguments)
		{
			FLinearColor back_color;

			FVector2D CurrentSize = FVector2D(4.0, 56.0);

			bool Hovered = this->CheckHovered(this->Position + CurrentSize, FVector2D(180.0, 20.0));
			bool Clicked = this->LeftMouseClicked && Hovered;
			if (Clicked)
			{
				back_color = FromRGB(49, 63, 68);
			}

			this->Offset.Y += 5.0;
			CurrentSize += this->Offset;
			this->Offset.Y += 23.0;

			back_color = FromRGB(39, 53, 58);

			Wrapper::GradientRectFilled(FVector2D(this->Position.X + CurrentSize.X + 4.0, this->Position.Y + CurrentSize.Y), FVector2D(180.0, 20.0), FromRGB(39, 53, 58), FromRGB(29, 38, 43));

			Wrapper::Line(FVector2D(this->Position.X + CurrentSize.X + 4.f, this->Position.Y + CurrentSize.Y), FVector2D(this->Position.X + 4.f + (CurrentSize.X + 180.0), this->Position.Y + CurrentSize.Y), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D(this->Position.X + CurrentSize.X + 4.f, this->Position.Y + CurrentSize.Y), FVector2D(this->Position.X + CurrentSize.X + 4.f, this->Position.Y + (CurrentSize.Y + 20.0)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D(this->Position.X + (CurrentSize.X + 4.f + 180.0), this->Position.Y + CurrentSize.Y), FVector2D(this->Position.X + (CurrentSize.X + 4.f + 180.0), this->Position.Y + (CurrentSize.Y + 20.0)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D(this->Position.X + CurrentSize.X + 4.f, this->Position.Y + (CurrentSize.Y + 20.0)), FVector2D(this->Position.X + (CurrentSize.X + 4.f + 180.0), this->Position.Y + (CurrentSize.Y + 20.0)), FromRGB(31, 37, 39), 1.f);

			Wrapper::Text(FString(Arguments[*Value]), FVector2D((this->Position.X + CurrentSize.X) + (180.0 / 2), (this->Position.Y + CurrentSize.Y) + (20.0 / 2) - 1), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, true, true);

			Wrapper::Line(FVector2D(this->Position.X + (CurrentSize.X + 4.f + 20.f), this->Position.Y + CurrentSize.Y), FVector2D(this->Position.X + (CurrentSize.X + 4.f + 20.f), this->Position.Y + (CurrentSize.Y + 20.0)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Text(FString(L"<"), FVector2D((this->Position.X + CurrentSize.X) + 12.f, (this->Position.Y + CurrentSize.Y) + (20.0 / 2) - 1), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, true, true);

			Wrapper::Line(FVector2D(this->Position.X + (CurrentSize.X + 4.f + (180.0 - 20.f)), this->Position.Y + CurrentSize.Y), FVector2D(this->Position.X + (CurrentSize.X + 4.f + (180.0 - 20.f)), this->Position.Y + (CurrentSize.Y + 20.0)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Text(FString(L">"), FVector2D((this->Position.X + CurrentSize.X + 180.0) - 8.f, (this->Position.Y + CurrentSize.Y) + (20.0 / 2) - 1), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, true, true);

			if (this->Cursor.X > this->Position.X + CurrentSize.X && this->Cursor.Y > this->Position.Y + CurrentSize.Y && this->Cursor.X < this->Position.X + (CurrentSize.X + 20.f) && this->Cursor.Y < this->Position.Y + (CurrentSize.Y + 20.0))
			{
				if (this->LeftMouseClicked)
				{
					*Value -= 1;

					if (*Value < 0)
					{
						*Value = NumArguments - 1;
					}
				}
			}

			if (this->Cursor.X > this->Position.X + CurrentSize.X + (180.0 - 20.f) && this->Cursor.Y > this->Position.Y + CurrentSize.Y && this->Cursor.X < this->Position.X + (CurrentSize.X + 180.0) && this->Cursor.Y < this->Position.Y + (CurrentSize.Y + 20.0))
			{
				if (this->LeftMouseClicked)
				{
					*Value += 1;

					if (*Value > NumArguments - 1)
					{
						*Value = 0;
					}
				}
			}

			Wrapper::Text(Content, FVector2D((this->Position.X + CurrentSize.X) + (180.0 + 10.0), this->Position.Y + CurrentSize.Y + (20.0 / 2) - 2), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, false, true);
		}

		void ColorPicker(FLinearColor* Color, bool* Active)
		{
			FVector2D CurrentSize = FVector2D(4.0, 56.0);
			double overall_box_size = 20.0;

			CurrentSize += this->Offset;

			Wrapper::RectFilled(FVector2D((this->Position.X + CurrentSize.X) + 264, (this->Position.Y + CurrentSize.Y) + 6), FVector2D(overall_box_size, overall_box_size - 8), *Color);

			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 264, (this->Position.Y + CurrentSize.Y) + 6), FVector2D(((this->Position.X + CurrentSize.X) + 264) + (overall_box_size), (this->Position.Y + CurrentSize.Y) + 6), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 264, (this->Position.Y + CurrentSize.Y) + 6), FVector2D((this->Position.X + CurrentSize.X) + 264, ((this->Position.Y + CurrentSize.Y) + 6) + (overall_box_size - 8)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D(((this->Position.X + CurrentSize.X) + 264) + (overall_box_size), (this->Position.Y + CurrentSize.Y) + 6), FVector2D(((this->Position.X + CurrentSize.X) + 264) + (overall_box_size), ((this->Position.Y + CurrentSize.Y) + 6) + (overall_box_size - 8)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 264, ((this->Position.Y + CurrentSize.Y) + 6) + (overall_box_size - 8)), FVector2D(((this->Position.X + CurrentSize.X) + 264) + (overall_box_size), ((this->Position.Y + CurrentSize.Y) + 6) + (overall_box_size - 8)), FromRGB(31, 37, 39), 1.f);

			bool IsHovered = this->Cursor.X > (this->Position.X + CurrentSize.X) + 264 && this->Cursor.Y > (this->Position.Y + CurrentSize.Y) + 6 && this->Cursor.X < ((this->Position.X + CurrentSize.X) + 264) + (overall_box_size) && this->Cursor.Y < ((this->Position.Y + CurrentSize.Y) + 6) + (overall_box_size - 8);
			if (IsHovered && LeftMouseClicked)
			{
				if (*Active)
					*Active = false;
				else if (!this->TotalOpenPopups)
					*Active = true;
			}

			FVector2D Current = FVector2D(((this->Position.X + CurrentSize.X) + 264) + (overall_box_size)+6, this->Position.Y);

			if (*Active)
			{
				this->TotalOpenPopups += 1;

				Current = FVector2D(Current.X + CurrentSize.X, Current.Y + CurrentSize.Y);
				double CurrentXBackup = Current.X;

				for (float c = 0; c < 20.f; c++)
				{
					float c_percentage = c / 20.f;
					float c_hx = 200.f * c_percentage;

					for (float i = 0; i < 10.f; i++)
					{
						float percentage = i / 10.f;
						float hx = 255.f * percentage;

						float red = 255.f - hx;
						float green = 255.f - red;
						float blue = 0.f;

						red = ColorPicker::ColorAddWhite(red, c_hx);
						green = ColorPicker::ColorAddWhite(green, c_hx);
						blue = ColorPicker::ColorAddWhite(blue, c_hx);

						FLinearColor converted_color = FromRGB(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));

						bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
						bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

						if (this_color_clicked)
						{
							*Color = converted_color;
							*Active = false;
						}

						Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
						Current.X += 10;
					}

					for (float i = 0; i < 10.f; i++)
					{
						float percentage = i / 10.f;
						float hx = 255.f * percentage;

						float red = 0.f;
						float green = 255.f - hx;
						float blue = 255.f - green;

						red = ColorPicker::ColorAddWhite(red, c_hx);
						green = ColorPicker::ColorAddWhite(green, c_hx);
						blue = ColorPicker::ColorAddWhite(blue, c_hx);

						FLinearColor converted_color = FromRGB(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));

						bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
						bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

						if (this_color_clicked)
						{
							*Color = converted_color;
							*Active = false;
						}

						Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
						Current.X += 10;
					}

					for (float i = 0; i < 10.f; i++)
					{
						float percentage = i / 10.f;
						float hx = 255.f * percentage;

						float green = 0.f;
						float blue = 255.f - hx;
						float red = 255.f - blue;

						red = ColorPicker::ColorAddWhite(red, c_hx);
						green = ColorPicker::ColorAddWhite(green, c_hx);
						blue = ColorPicker::ColorAddWhite(blue, c_hx);

						FLinearColor converted_color = FromRGB(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));

						bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
						bool this_color_clicked = this_color_hovered && this->LeftMouseClicked;

						if (this_color_clicked)
						{
							*Color = converted_color;
							*Active = false;
						}

						Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
						Current.X += 10;
					}

					Current.X = CurrentXBackup;
					Current.Y += 10;
				}

				Current.X = CurrentXBackup;

				for (float i = 0; i < 30.f; i++)
				{
					float percentage = i / 30.f;
					float hx = 255.f * percentage;

					float green = 255.f - hx;
					float blue = 255.f - hx;
					float red = 255.f - hx;

					FLinearColor converted_color = FromRGB(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));

					bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
					bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

					if (this_color_clicked)
					{
						*Color = converted_color;
						*Active = false;
					}

					Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
					Current.X += 10;
				}
			}
		}

		void CheckboxTest(FString text, bool* Value)
		{
			FVector2D CurrentSize = FVector2D(4.0, 56.0);

			CurrentSize += this->Offset;
			this->Offset.Y += 20.0;

			double overall_box_size = 20.0;

			bool Hovered = this->CheckHovered(this->Position + FVector2D(CurrentSize.X, CurrentSize.Y), FVector2D(overall_box_size, overall_box_size));
			bool Clicked = this->LeftMouseClicked && Hovered;

			FLinearColor back_color = FromRGB(39, 53, 58);
			FLinearColor back_color_g = FromRGB(29, 38, 43);

			if (*Value)
			{
				back_color = FromRGB(67, 236, 16);
				back_color_g = FromRGB(24, 226, 51);
			}

			if (Clicked)
				*Value = !*Value;


			if (!Value)
			{
				Wrapper::GradientRectFilled(FVector2D((this->Position.X + CurrentSize.X) + 4,
					(this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(overall_box_size - 6.0, overall_box_size - 6.0), back_color, back_color_g);
			}
			else
			{
				Wrapper::GradientRectFilled(
					FVector2D((this->Position.X + CurrentSize.X) + 4,
					(this->Position.Y + CurrentSize.Y) + 8.0), 
					FVector2D(overall_box_size - 12.0, overall_box_size - 12.0), 
					back_color, 
					back_color_g);
			}

			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 4.0, (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6.f), (this->Position.Y + CurrentSize.Y) + 4.0), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 4.0, (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D((this->Position.X + CurrentSize.X) + 4.0, ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6), (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6.f), ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 4.0, ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6.f), ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FromRGB(31, 37, 39), 1.f);

			Wrapper::Text(text, FVector2D((this->Position.X + CurrentSize.X) + (overall_box_size + 2.f), this->Position.Y + CurrentSize.Y + (overall_box_size / 2.f)), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, false, true);
		}

		void Checkbox(FString text, bool* Value)
		{
			CheckboxTest(text, Value);

			/*FVector2D CurrentSize = FVector2D(4.0, 56.0);

			CurrentSize += this->Offset;
			this->Offset.Y += 20.0;

			double overall_box_size = 20.0;

			bool Hovered = this->CheckHovered(this->Position + FVector2D(CurrentSize.X, CurrentSize.Y), FVector2D(overall_box_size, overall_box_size));
			bool Clicked = this->LeftMouseClicked && Hovered;

			FLinearColor back_color = FromRGB(39, 53, 58);
			FLinearColor back_color_g = FromRGB(29, 38, 43);

			if (*Value)
			{
				back_color = FromRGB(67, 236, 16);
				back_color_g = FromRGB(24, 226, 51);
			}

			if (Clicked)
				*Value = !*Value;


			Wrapper::GradientRectFilled(FVector2D((this->Position.X + CurrentSize.X) + 4,
					(this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(overall_box_size - 6.0, overall_box_size - 6.0), back_color, back_color_g);

			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 4.0, (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6.f), (this->Position.Y + CurrentSize.Y) + 4.0), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 4.0, (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D((this->Position.X + CurrentSize.X) + 4.0, ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6), (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6.f), ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FromRGB(31, 37, 39), 1.f);
			Wrapper::Line(FVector2D((this->Position.X + CurrentSize.X) + 4.0, ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FVector2D(((this->Position.X + CurrentSize.X) + 4.0) + (overall_box_size - 6.f), ((this->Position.Y + CurrentSize.Y) + 4.0) + (overall_box_size - 6.f)), FromRGB(31, 37, 39), 1.f);

			Wrapper::Text(text, FVector2D((this->Position.X + CurrentSize.X) + (overall_box_size + 2.f), this->Position.Y + CurrentSize.Y + (overall_box_size / 2.f)), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, false, true);
			*/
		}


		void Slider(FString Content, double* Value, double Min, double Max)
		{
			double overall_box_size = 14.0;
			double slider_width = 200.0;

			FVector2D CurrentSize = FVector2D(4.0, 56.0);

			this->Offset.Y += 5.0;
			CurrentSize += this->Offset;
			this->Offset.Y += 16.0;

			Wrapper::RectFilled(FVector2D((this->Position.X + CurrentSize.X) + 4.0, (this->Position.Y + CurrentSize.Y) + 3.0), FVector2D(slider_width - 6.0, overall_box_size - 6.0), FromRGB(31, 37, 39));

			Wrapper::GradientRectFilled(FVector2D((this->Position.X + CurrentSize.X) + 5.0, (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(slider_width - 8.f, overall_box_size - 8.0), FromRGB(39, 53, 58), FromRGB(29, 38, 43));

			Wrapper::GradientRectFilled(FVector2D((this->Position.X + CurrentSize.X) + 5.0, (this->Position.Y + CurrentSize.Y) + 4.0), FVector2D(*Value * ((slider_width - 8.0) / Max), overall_box_size - 8.0), FromRGB(67, 236, 16), FromRGB(24, 226, 51));

			FVector2D Size = FVector2D(120, 10); //FVector2D Size = FVector2D(125.0, 12.0);

			bool Hovered = this->CheckHovered(this->Position + CurrentSize, FVector2D(slider_width, overall_box_size));
			bool Down = this->LeftMouseDown && Hovered;

			if (Hovered && Down && !this->TotalOpenPopups)
			{
				if (this->Cursor.X < this->Position + (CurrentSize.X + 4.0))
				{
					*Value = Min;
				}
				else if (this->Cursor.X > this->Position.X + (CurrentSize.X + (slider_width - 8.0)))
				{
					*Value = Max;
				}
				else
				{
					*Value = (this->Cursor.X - (this->Position.X + (CurrentSize.X + 4.0))) * (Max / (slider_width - 8.0));
				}

				Wrapper::Text(StaticClasses::KismetStringLibrary->Conv_IntToString(static_cast<int>(*Value)), FVector2D((this->Position.X + CurrentSize.X) + (*Value * ((slider_width - 8.0) / Max)), ((this->Position.Y + CurrentSize.Y) + 2.0)), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, true, false);
			}
		
			FString ConvertedContent = StaticClasses::KismetStringLibrary->BuildString_Int(Content, FString(L": "), int(*Value), FString(L""));

			Wrapper::Text(ConvertedContent, FVector2D((this->Position.X + CurrentSize.X) + (slider_width + 5.0), this->Position.Y + CurrentSize.Y + (overall_box_size / 2.0) - 2.0), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, false, true);
		}

		bool Button(FString text, double _x, double _y, double _width, double _height, bool outline, FLinearColor override_color = FLinearColor{ -1, -1, -1, -1 })
		{
			FLinearColor RenderColor;
			bool Hovered = this->CheckHovered(this->Position + FVector2D(_x, _y), FVector2D(_width, _height));
			bool Clicked = this->LeftMouseClicked && Hovered;
			bool Down = this->LeftMouseDown && Hovered;

			if (Down)
				RenderColor = FromRGB(49, 63, 68);
			else
				RenderColor = FromRGB(39, 53, 58);

			if (override_color.A != -1 && override_color.G != -1 && override_color.G != -1 && override_color.B != -1)
			{
				RenderColor = override_color;
			}

			Wrapper::RectFilled(FVector2D(this->Position.X + _x + 1, this->Position.Y + _y + 1), FVector2D(_width - 2, _height - 2), RenderColor);

			if (outline)
			{
				Wrapper::Line(FVector2D{ this->Position.X + _x, this->Position.Y + _y }, FVector2D{ this->Position.X + (_x + _width), this->Position.Y + _y }, FLinearColor{ 0.05, 0.05, 0.05, 1 }, 1.f);
				Wrapper::Line(FVector2D{ this->Position.X + _x, this->Position.Y + _y }, FVector2D{ this->Position.X + _x, this->Position.Y + (_y + _height) }, FLinearColor{ 0.05, 0.05, 0.05, 1 }, 1.f);
				Wrapper::Line(FVector2D{ this->Position.X + (_x + _width), this->Position.Y + _y }, FVector2D{ this->Position.X + (_x + _width), this->Position.Y + (_y + _height) }, FLinearColor{ 0.05, 0.05, 0.05, 1 }, 1.f);
				Wrapper::Line(FVector2D{ this->Position.X + _x, this->Position.Y + (_y + _height) }, FVector2D{ this->Position.X + (_x + _width), this->Position.Y + (_y + _height) }, FLinearColor{ 0.05, 0.05, 0.05, 1 }, 1.f);
			}

			Wrapper::Text(text, FVector2D((this->Position.X + _x) + (_width / 2) - 2, (this->Position.Y + _y) + (_height / 2) - 2), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, true, true);

			return Clicked && !this->TotalOpenPopups;
		}

		void TabsTest(const wchar_t* names[], int array_size, int* tab)
		{
			const int tab_count = array_size;
			double tab_width = ((this->Size.X - 4) / tab_count) + 15.0; // spacing = 15

			double tab_height = 26.f;

			for (int t = 0; t < tab_count; t++)
			{
				FLinearColor override_color = FLinearColor{ -1, -1, -1, -1 };

				if (*tab == t)
				{
					override_color = FromRGB(35, 46, 50);
				}

				if (Button(FString(names[t]), 2 + (tab_width * t), 26.f, tab_width + 1.f, tab_height + 2, false, override_color))
				{
					*tab = t;
				}

				if (*tab == 0 || *tab == array_size - 1)
				{
					if (*tab == 0)
					{
						Wrapper::Line(FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
					}
					if (*tab == array_size - 1)
					{
						Wrapper::Line(FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
					}
				}
				else
				{
					Wrapper::Line(FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
					Wrapper::Line(FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
				}

				if (*tab != t)
				{
					Wrapper::Line(FVector2D{ this->Position.X + (2 + (tab_width * t)), this->Position.Y + (26.f + tab_height) }, FVector2D{ (this->Position.X + (2 + (tab_width * t))) + tab_width - 1, this->Position.Y + (26.f + tab_height) }, FromRGB(31, 37, 39), 1.f);
				}
			}
		}


		void Tabs(const wchar_t* names[], int array_size, int* tab)
		{
			const int tab_count = array_size;
			double tab_width = ((this->Size.X - 4) / tab_count);

			double tab_height = 26.f;

			for (int t = 0; t < tab_count; t++)
			{
				FLinearColor override_color = FLinearColor{ -1, -1, -1, -1 };

				if (*tab == t)
				{
					override_color = FromRGB(35, 46, 50);
				}

				if (Button(FString(names[t]), 2 + (tab_width * t), 26.f, tab_width + 1.f, tab_height + 2, false, override_color))
				{
					*tab = t;
				}

				if (*tab == 0 || *tab == array_size - 1)
				{
					if (*tab == 0)
					{
						Wrapper::Line(FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
					}
					if (*tab == array_size - 1)
					{
						Wrapper::Line(FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
					}
				}
				else
				{
					Wrapper::Line(FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + ((2 + (tab_width * *tab)) + tab_width), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
					Wrapper::Line(FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + (26.f) }, FVector2D{ this->Position.X + (2 + (tab_width * *tab)), this->Position.Y + ((26.f) + tab_height) }, FromRGB(31, 37, 39), 1.f);
				}

				if (*tab != t)
				{
					Wrapper::Line(FVector2D{ this->Position.X + (2 + (tab_width * t)), this->Position.Y + (26.f + tab_height) }, FVector2D{ (this->Position.X + (2 + (tab_width * t))) + tab_width - 1, this->Position.Y + (26.f + tab_height) }, FromRGB(31, 37, 39), 1.f);
				}
			}
		}

		void Watermark(FString Text)
		{
			FVector2D Size = FVector2D(5.0, 5.0);

			Wrapper::RectFilled(Size, FVector2D(260.0, 25), FromRGB(35, 46, 50), true);

			Wrapper::Line(Size, FVector2D(Size.X + 260.0, Size.Y), FromRGB(52, 63, 80), 1.f, true);
			Wrapper::Line(Size, FVector2D(Size.X, Size.Y + 25.0), FromRGB(52, 63, 80), 1.f, true);
			Wrapper::Line(FVector2D(Size.X + 260.0, Size.Y), FVector2D(Size.X + 260.0, Size.Y + 25.0), FromRGB(52, 63, 80), 1.f, true);
			Wrapper::Line(FVector2D(Size.X, Size.Y + 25.0), FVector2D(Size.X + 260.0, Size.Y + 25.0), FromRGB(52, 63, 80), 1.f, true);

			Wrapper::Text(Text, FVector2D(Size.X + 5.0, Size.Y + 13.0), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, false, true, false, true);

			Wrapper::Line(FVector2D(Size.X + 2.0, Size.Y + 2.0), FVector2D(Size.X + 260.0 - 3, Size.Y + 2), FromRGB(67, 236, 16), 2.f, true);
		}

		bool CreateMenu()
		{
			if (!this->AlreadyInitialized)
			{
				this->Size = FVector2D(584.0, 490.0);
				//this->Size = FVector2D(584.0, 490.0);

				this->Position = FVector2D(200.0, 200.0);

				this->AlreadyInitialized = true;
			}

			this->TotalOpenPopups = 0;
			this->Offset = FVector2D(0.0, 0.0);

			if (FrameUpdated != Variables::CurrentFrame)
			{
				this->Cursor = Variables::PlayerController->GetMousePosition();
				this->LeftMouseClicked = Variables::PlayerController->WasInputKeyJustPressed(Keys::LeftMouseButton);
				this->LeftMouseDown = Variables::PlayerController->IsInputKeyDown(Keys::LeftMouseButton);
				this->RightMouseClicked = Variables::PlayerController->WasInputKeyJustPressed(Keys::RightMouseButton);
				this->RightMouseDown = Variables::PlayerController->IsInputKeyDown(Keys::RightMouseButton);

				FrameUpdated = Variables::CurrentFrame;
			}

			static FVector2D ClickPosition = FVector2D(0.0, 0.0);
			WindowHovered = this->CheckHovered(FVector2D(this->Position.X, this->Position.Y - 5.0), FVector2D(this->Size.X, 20.0));
			WindowGrabbed = WindowGrabbed ? LeftMouseDown : WindowHovered && LeftMouseDown;
			if (WindowGrabbed)
			{
				if (!ClickPosition)
				{
					ClickPosition = FVector2D(this->Cursor.X - this->Position.X, this->Cursor.Y - this->Position.Y);
				}

				WindowHovered = true;
			}

			if (ClickPosition)
			{
				this->Position = FVector2D(this->Cursor.X - (this->Size.X / 2), this->Cursor.Y - (20.0 / 2.0));
			}

			if (!this->LeftMouseDown)
			{
				ClickPosition = FVector2D();
			}

			Wrapper::RectFilled(this->Position, FVector2D(this->Size.X, 20.0), FromRGB(24, 28, 30));

			Wrapper::Line(this->Position, FVector2D(this->Position.X + this->Size.X, this->Position.Y), FromRGB(52, 63, 80), 1.f);
			Wrapper::Line(this->Position, FVector2D(this->Position.X, this->Position.Y + 20), FromRGB(52, 63, 80), 1.f);
			Wrapper::Line(FVector2D(this->Position.X + this->Size.X, this->Position.Y), FVector2D(this->Position.X + this->Size.X, this->Position.Y + 20), FromRGB(52, 63, 80), 1.f);
			Wrapper::Line(FVector2D(this->Position.X, this->Position.Y + 20), FVector2D(this->Position.X + this->Size.X, this->Position.Y + 20), FromRGB(52, 63, 80), 1.f);

			static auto Enc_WataMark = Encrypt(L"Vixen Free - discord.gg/vixencommunity - t.me/vixenud");
			//static auto Enc_WataMark = Encrypt(L"SpoofX Free - discord.gg/spoofx");
			static FString WaltaMark = FString(Enc_WataMark.decrypt()); // we need to cook ( i need to cook and need head fr)

			Wrapper::Text(FString(WaltaMark), FVector2D(this->Position.X + (this->Size.X / 2.0), this->Position.Y + 10.0), FLinearColor(1.f, 1.f, 1.f, 1.f), 10.0, true, true);

			this->Position.Y += 22.0;

			Wrapper::RectFilled(this->Position, this->Size, FromRGB(31, 37, 39));

			Wrapper::Line(this->Position, FVector2D(this->Position.X + this->Size.X, this->Position.Y), FromRGB(52, 63, 80), 1.f);
			Wrapper::Line(this->Position, FVector2D(this->Position.X, this->Position.Y + this->Size.Y), FromRGB(52, 63, 80), 1.f);
			Wrapper::Line(FVector2D(this->Position.X + this->Size.X, this->Position.Y), FVector2D(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y), FromRGB(52, 63, 80), 1.f);
			Wrapper::Line(FVector2D(this->Position.X, this->Position.Y + this->Size.Y), FVector2D(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y), FromRGB(52, 63, 80), 1.f);

			Wrapper::RectFilled(FVector2D(this->Position.X + 2.0, this->Position.Y + 2.0), FVector2D(this->Size.X - 4.0, this->Size.Y - 4.0), FromRGB(24, 28, 30));
			Wrapper::Line(FVector2D(this->Position.X + 2.0, this->Position.Y + 2.0), FVector2D(this->Position.X + this->Size.X - 3.0, this->Position.Y + 2.0), FromRGB(67, 236, 16), 2.f);

			this->Position.Y -= 22.0;

			return true;
		}

		void RenderMouse()
		{
			Wrapper::RectFilled(FVector2D(this->Cursor.X - 4.0, this->Cursor.Y - 4.0), FVector2D(4.0, 4.0), Settings::Colors::BoxVisible, true);
		}

	private:
		bool AlreadyInitialized, LeftMouseClicked, RightMouseClicked, LeftMouseDown, RightMouseDown; bool WindowHovered = false; bool WindowGrabbed = false;
		FVector2D Position, Size, Offset, Cursor, ScreenCenter;
		int TotalOpenPopups; double CurrentAlpha = 0.0;
		uint64 FrameUpdated = 0;
	};
}