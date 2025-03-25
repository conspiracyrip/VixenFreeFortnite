#pragma once

namespace Render
{
	void Line(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FLinearColor Color, float Thickness)
	{
		//Variables::Canvas->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness, Color);
	}

	void Text(FString Content, FVector2D ScreenPosition, double FontSize, FLinearColor Color, bool CenterX, bool CenterY, bool Outlined)
	{
		Variables::Canvas->K2_DrawText(Content, ScreenPosition, FontSize, Color, CenterX, CenterY, Outlined);
	}

	void Diamond(FVector2D ScreenPosition, FVector2D Radius, FLinearColor RenderColor)
	{
		FVector2D DiamondPoints[4];
		DiamondPoints[0] = { ScreenPosition.X, ScreenPosition.Y - Radius.Y };
		DiamondPoints[1] = { ScreenPosition.X + Radius.X, ScreenPosition.Y };
		DiamondPoints[2] = { ScreenPosition.X, ScreenPosition.Y + Radius.Y };
		DiamondPoints[3] = { ScreenPosition.X - Radius.X, ScreenPosition.Y };

		Variables::Canvas->K2_DrawPolygon(nullptr, DiamondPoints[0], Radius, 4, RenderColor);
	}

	void DrawRectFilled(FVector2D Position, FVector2D Size, FLinearColor RenderColor)
	{
		for (double i = 0; i < Size.Y; i++)
		{
			Render::Line(FVector2D(Position.X, Position.Y + i), FVector2D(Position.X + Size.X, Position.Y + i), RenderColor, 1.f);
		}
	}

	void DrawGradientRectFilled(FVector2D Position, FVector2D Size, FLinearColor ColorA, FLinearColor ColorB)
	{
		for (double a = Position.Y; a < Position.Y + Size.Y; a++)
		{
			float Alpha = (a - Position.Y) / Size.Y;
			FLinearColor InterpolatedColor = StaticClasses::KismetMathLibrary->LinearColorLerpUsingHSV(ColorA, ColorB, Alpha);
			Render::Line(FVector2D(Position.X, a), FVector2D(Position.X + Size.X, a), InterpolatedColor, 1.f);
		}
	}

	void DrawRectRounded(FVector2D ScreenPosition, FVector2D Size, FLinearColor RenderColor, float Rounding, bool Filled)
	{
		if (Filled)
		{
			Render::DrawRectFilled(FVector2D(ScreenPosition.X + Rounding, ScreenPosition.Y), FVector2D(Size.X - (Rounding * 2.0), Size.Y), RenderColor);
			Render::DrawRectFilled(FVector2D(ScreenPosition.X, ScreenPosition.Y + Rounding), FVector2D(Size.X, Size.Y - (Rounding * 2.0)), RenderColor);
		}

		Render::Diamond(FVector2D(ScreenPosition.X + Rounding - 2.0, ScreenPosition.Y + Rounding + 3.0), FVector2D(Rounding - 1.0, Rounding - 2.0), RenderColor);
		Render::Diamond(FVector2D(ScreenPosition.X + Size.X - Rounding + 1.0, ScreenPosition.Y + Rounding + 3.0), FVector2D(Rounding - 1.0, Rounding - 2.0), RenderColor);
		Render::Diamond(FVector2D(ScreenPosition.X + Rounding - 2.0, ScreenPosition.Y + Size.Y - Rounding + 5.0), FVector2D(Rounding - 1.0, Rounding - 2.0), RenderColor);
		Render::Diamond(FVector2D(ScreenPosition.X + Size.X - Rounding + 1.0, ScreenPosition.Y + Size.Y - Rounding + 5.0), FVector2D(Rounding - 1.0, Rounding - 2.0), RenderColor);
	}

	void DrawRect(FVector2D ScreenPosition, FVector2D Size, FLinearColor RenderColor, float Thickness)
	{
		Render::Line(FVector2D(ScreenPosition.X, ScreenPosition.Y), FVector2D(ScreenPosition.X + Size.X, ScreenPosition.Y), RenderColor, Thickness);
		Render::Line(FVector2D(ScreenPosition.X + Size.X, ScreenPosition.Y), FVector2D(ScreenPosition.X + Size.X, ScreenPosition.Y + Size.Y), RenderColor, Thickness);
		Render::Line(FVector2D(ScreenPosition.X + Size.X, ScreenPosition.Y + Size.Y), FVector2D(ScreenPosition.X, ScreenPosition.Y + Size.Y), RenderColor, Thickness);
		Render::Line(FVector2D(ScreenPosition.X, ScreenPosition.Y + Size.Y), FVector2D(ScreenPosition.X, ScreenPosition.Y), RenderColor, Thickness);
	}

	void Triangle(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FVector2D ScreenPositionC, FLinearColor RenderColor, float Thickness, bool Filled)
	{
		if (Filled)
		{
			// TODO: Fix
		}
		else
		{
			Render::Line(ScreenPositionA, ScreenPositionB, RenderColor, Thickness);
			Render::Line(ScreenPositionB, ScreenPositionC, RenderColor, Thickness);
			Render::Line(ScreenPositionC, ScreenPositionA, RenderColor, Thickness);
		}
	}

	void Circle(FVector2D ScreenPosition, FLinearColor color, double radius, double segments, bool filled)
	{
		double Step = M_PI * 2.0 / segments;
		int Count = 0;
		FVector2D Vectors[128];

		for (double Degree = 0; Degree < M_PI * 2.0; Degree += Step)
		{
			double X_1 = radius * StaticClasses::KismetMathLibrary->cos(Degree) + ScreenPosition.X;
			double Y_1 = radius * StaticClasses::KismetMathLibrary->sin(Degree) + ScreenPosition.Y;
			double X_2 = radius * StaticClasses::KismetMathLibrary->cos(Degree + Step) + ScreenPosition.X;
			double Y_2 = radius * StaticClasses::KismetMathLibrary->sin(Degree + Step) + ScreenPosition.Y;

			Vectors[Count].X = X_1;
			Vectors[Count].Y = Y_1;
			Vectors[Count + 1].X = X_2;
			Vectors[Count + 1].Y = Y_2;

			if (filled)
			{
				for (double X = X_1; X <= X_2; X += 1.0)
				{
					double Y = ScreenPosition.Y + radius * StaticClasses::KismetMathLibrary->sin(StaticClasses::KismetMathLibrary->acos((X - ScreenPosition.X) / radius));
					Render::Line(FVector2D(X, Y_1), FVector2D(X, Y), color, 1.0f);
					Render::Line(FVector2D(X, Y), FVector2D(X, Y_2), color, 1.0f);
				}
			}
			else
			{
				Render::Line(FVector2D(Vectors[Count].X, Vectors[Count].Y), FVector2D(X_2, Y_2), color, 1.0f);
			}
		}
	}
}

namespace Wrapper
{
	struct DrawListMember
	{
		int DrawType;

		FString Content;

		FVector2D PositionA, PositionB, PositionC;
		FLinearColor Color;
		FLinearColor ColorB;

		float Thickness;

		double Radius;
		double Segments;
		double Size;
		bool Filled;

		bool Outlined;
		bool CenteredX;
		bool CenteredY;

		bool Rounded;
	};

	int LastForegroundIndex = 0;
	DrawListMember ForegroundDrawList[9999];

	int LastDefaultIndex = 0;
	DrawListMember DefaultDrawList[9999];

	void ClearTargets()
	{
		LastDefaultIndex = 0;
		LastForegroundIndex = 0;
	}

	bool IsSafeToRender()
	{
		bool ReturnValue = Variables::Canvas && Variables::Roboto;
		if (!ReturnValue)
		{
			ClearTargets();
		}

		return ReturnValue;
	}

	void Tick()
	{
		for (int i = 0; i < LastDefaultIndex; i++)
		{
			DrawListMember Member = DefaultDrawList[i];

			if (Member.DrawType == 1)
			{
				Render::Line(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 2)
			{
				Render::DrawRect(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 3)
			{
				Render::DrawRectFilled(Member.PositionA, Member.PositionB, Member.Color);
			}
			else if (Member.DrawType == 4)
			{
				Render::Text(Member.Content, Member.PositionA, Member.Size, Member.Color, Member.CenteredX, Member.CenteredY, Member.Outlined);
			}
			else if (Member.DrawType == 5)
			{
				Render::Circle(Member.PositionA, Member.Color, Member.Radius, Member.Segments, Member.Filled);
			}
			else if (Member.DrawType == 6)
			{
				Render::DrawRectRounded(Member.PositionA, Member.PositionB, Member.Color, Member.Segments, Member.Filled);
			}
			else if (Member.DrawType == 7)
			{
				Render::Triangle(Member.PositionA, Member.PositionB, Member.PositionC, Member.Color, Member.Thickness, Member.Filled);
			}
			else if (Member.DrawType == 8)
			{
				Render::DrawGradientRectFilled(Member.PositionA, Member.PositionB, Member.Color, Member.ColorB);
			}
		}

		for (int i = 0; i < LastForegroundIndex; i++)
		{
			DrawListMember Member = ForegroundDrawList[i];

			if (Member.DrawType == 1)
			{
				Render::Line(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 2)
			{
				Render::DrawRect(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 3)
			{
				Render::DrawRectFilled(Member.PositionA, Member.PositionB, Member.Color);
			}
			else if (Member.DrawType == 4)
			{
				Render::Text(Member.Content, Member.PositionA, Member.Size, Member.Color, Member.CenteredX, Member.CenteredY, Member.Outlined);
			}
			else if (Member.DrawType == 5)
			{
				Render::Circle(Member.PositionA, Member.Color, Member.Radius, Member.Segments, Member.Filled);
			}
			else if (Member.DrawType == 6)
			{
				Render::DrawRectRounded(Member.PositionA, Member.PositionB, Member.Color, Member.Segments, Member.Filled);
			}
			else if (Member.DrawType == 7)
			{
				Render::Triangle(Member.PositionA, Member.PositionB, Member.PositionC, Member.Color, Member.Thickness, Member.Filled);
			}
			else if (Member.DrawType == 8)
			{
				Render::DrawGradientRectFilled(Member.PositionA, Member.PositionB, Member.Color, Member.ColorB);
			}
		}

		Wrapper::ClearTargets();
	}

	void Line(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, float Thickness, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 1;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Thickness = Thickness;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Rect(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, float Thickness, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 2;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Thickness = Thickness;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void RectFilled(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 3;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void GradientRectFilled(FVector2D PositionA, FVector2D PositionB, FLinearColor StartColor, FLinearColor EndColor, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 8;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = StartColor;
		Member.ColorB = EndColor;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void RoundedRect(FVector2D PositionA, FVector2D PositionB, float Segments, FLinearColor RenderColor, bool Filled = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 6;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Segments = Segments;
		Member.Color = RenderColor;
		Member.Filled = Filled;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Text(FString Content, FVector2D Position, FLinearColor RenderColor, double Size = Settings::Misc::FontSize, bool CenteredX = false, bool CenteredY = false, bool Outlined = Settings::Misc::TextOutline, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 4;
		Member.Content = Content;
		Member.PositionA = Position;
		Member.Size = Size;
		Member.Color = RenderColor;
		Member.CenteredX = CenteredX;
		Member.CenteredY = CenteredY;
		Member.Outlined = Outlined;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	// Function to convert HSV to RGB (FLinearColor)
	FLinearColor HSVtoRGB(float h, float s, float v)
	{
		float r, g, b;

		int i = static_cast<int>(h * 6);
		float f = h * 6 - i;
		float p = v * (1 - s);
		float q = v * (1 - f * s);
		float t = v * (1 - (1 - f) * s);

		switch (i % 6)
		{
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		// Return as FLinearColor with full alpha (1.0f)
		return FLinearColor(r, g, b, 1.0f);
	}

	void Circle(FVector2D Position, FLinearColor RenderColor, double Radius, double Segments, bool Filled = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 5;
		Member.PositionA = Position;
		Member.Color = RenderColor;
		Member.Radius = Radius;
		Member.Segments = Segments;
		Member.Filled = Filled;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Triangle(FVector2D PositionA, FVector2D PositionB, FVector2D PositionC, float Thickness, FLinearColor RenderColor, bool Filled = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 7;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.PositionC = PositionC;
		Member.Thickness = Thickness;
		Member.Color = RenderColor;
		Member.Filled = Filled;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}
}

namespace Radar
{
	void Range(double* X, double* Y, double Range)
	{
		if (StaticClasses::KismetMathLibrary->abs((*X)) > Range || StaticClasses::KismetMathLibrary->abs((*Y)) > Range)
		{
			if ((*Y) > (*X))
			{
				if ((*Y) > -(*X))
				{
					(*X) = Range * (*X) / (*Y);
					(*Y) = Range;
				}
				else
				{
					(*Y) = -Range * (*Y) / (*X);
					(*X) = -Range;
				}
			}
			else
			{
				if ((*Y) > -(*X))
				{
					(*Y) = Range * (*Y) / (*X);
					(*X) = Range;
				}
				else
				{
					(*X) = -Range * (*X) / (*Y);
					(*Y) = -Range;
				}
			}
		}
	}

	void RotatePoint(FVector2D* Screen, FVector Origin, FVector CameraLocation, FRotator CameraRotation, FVector2D Size, FVector2D Position)
	{
		FVector Relative = Origin - CameraLocation;

		double Yaw = CameraRotation.Yaw * 3.1415927 / 180.0;

		double fsin_yaw = StaticClasses::KismetMathLibrary->sin(Yaw);
		double fminus_cos_yaw = -StaticClasses::KismetMathLibrary->cos(Yaw);

		double X = -(Relative.Y * fminus_cos_yaw + Relative.X * fsin_yaw);
		double Y = Relative.X * fminus_cos_yaw - Relative.Y * fsin_yaw;

		double RangeValue = 30 * 1000;
		Range(&X, &Y, RangeValue);

		FVector2D ScreenPosition = FVector2D(Settings::Player::RadarX, Settings::Player::RadarY);
		FVector2D MaxScreenPosition = FVector2D(Size.X + ScreenPosition.X - 5.0, Size.Y + ScreenPosition.Y - 5.0);
		FVector2D OutScreenPosition = FVector2D(ScreenPosition.X + Size.X / 2.0 + (X / RangeValue * Size.X), ScreenPosition.Y + (Size.Y / 2.0 + (Y / RangeValue * Size.Y)));

		//if (OutScreenPosition.X > MaxScreenPosition.X)
		//	OutScreenPosition.X = MaxScreenPosition.X;

		//if (OutScreenPosition.X < ScreenPosition.X)
		//	OutScreenPosition.X = ScreenPosition.X;

		//if (OutScreenPosition.Y > ScreenPosition.Y)
		//	OutScreenPosition.Y = ScreenPosition.Y;

		//if (OutScreenPosition.Y < ScreenPosition.Y)
		//	OutScreenPosition.Y = ScreenPosition.Y;

		*Screen = OutScreenPosition;
	}

	void Setup(FVector2D RadarPos, FVector2D RadarSize)
	{
		Wrapper::GradientRectFilled(RadarPos, RadarSize, FLinearColor(0.f, 0.f, 0.f, 0.f), FLinearColor(0.025f, 0.025f, 0.025f, 1.f), true);

		Wrapper::Line(FVector2D(RadarPos.X + RadarSize.X / 2, RadarPos.Y + RadarSize.Y), FVector2D(RadarPos.X + RadarSize.X / 2, RadarPos.Y + RadarSize.Y / 2), FLinearColor(1.f, 1.f, 1.f, 1.f), 1.f, true);
		Wrapper::Line(FVector2D(RadarPos.X + RadarSize.X / 2, RadarPos.Y + RadarSize.Y / 2), FVector2D(RadarPos.X, RadarPos.Y), FLinearColor(1.f, 1.f, 1.f, 1.f), 1.f, true);
		Wrapper::Line(FVector2D(RadarPos.X + RadarSize.X / 2, RadarPos.Y + RadarSize.Y / 2), FVector2D(RadarPos.X + RadarSize.X, RadarPos.Y), FLinearColor(1.f, 1.f, 1.f, 1.f), 1.f, true);
	}

	void Add(FVector WorldLocation, FLinearColor Color, FVector2D RadarPos, FVector2D RadarSize)
	{
		FVector2D Projected = FVector2D();
		Radar::RotatePoint(&Projected, WorldLocation, Variables::CameraLocation, Variables::CameraRotation, RadarSize, RadarPos);
		Wrapper::RectFilled(Projected, FVector2D(6, 6), Color, true);
	}
}

enum VixenDebugMessageType : int
{
	Log,
	Info,
	Warn,
	Error,
	Debug,
	Misc,
	Other
};

namespace Custom
{

	FKey GetAimKey(int Key)
	{
		if (Settings::Aimbot::KeyType == 1)
			return Keys::LeftMouseButton;
		else if (Settings::Aimbot::KeyType == 2)
			return Keys::LeftShift;
		else if (Settings::Aimbot::KeyType == 3)
			return Keys::Other::ThumbMouseButton;
		else if (Settings::Aimbot::KeyType == 4)
			return Keys::Other::ThumbMouseButton2;

		return Keys::RightMouseButton;
	}

	bool K2_Project(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		FVector ReturnValue = Variables::Canvas->K2_Project(WorldLocation);

		if (ReturnValue.Z <= 0.0)
		{
			ReturnValue.X *= -1.0;
			ReturnValue.Y *= -1.0;

			ReturnValue.X += Variables::ScreenSize.X;
			ReturnValue.Y += Variables::ScreenSize.Y;
		}

		if (ReturnValue.Z > 0.0)
		{
			*ScreenLocation = FVector2D(ReturnValue.X, ReturnValue.Y);
		}

		return ScreenLocation;
	}

	FName GetNameFromString(FString InString)
	{
		static map<wchar_t*, FName> Names;
		auto it = Names.find(InString.c_str());
		if (it != Names.end())
			return it->second;

		FName OutName = StaticClasses::KismetStringLibrary->Conv_StringToName(InString);
		Names.insert(InString.c_str(), OutName);

		return OutName;
	}

	FVector GetSocketLocation(USkeletalMeshComponent* Mesh, FString InSocketName)
	{
		FName SocketName = Custom::GetNameFromString(InSocketName);

		return Mesh->GetSocketLocation(SocketName);
	}

	double InterpolatedValue(double CurrentScalar, double MaxScalar, double MinValue, double MaxValue)
	{
		MaxScalar = StaticClasses::KismetMathLibrary->FMin(MaxScalar, CurrentScalar);

		double InterpolatedValue = MaxValue - (MaxValue - MinValue) * (MaxScalar / CurrentScalar);

		InterpolatedValue = StaticClasses::KismetMathLibrary->Clamp(InterpolatedValue, MinValue, MaxValue);

		return InterpolatedValue;
	}

	bool InScreen(FVector2D ScreenLocation)
	{
		if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2) && ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2))
			return false;
		return true;
	}

	bool InRect(double Radius, FVector2D ScreenLocation)
	{
		return Variables::ScreenCenter.X >= (Variables::ScreenCenter.X - Radius) && Variables::ScreenCenter.X <= (Variables::ScreenCenter.X + Radius) &&
			ScreenLocation.Y >= (ScreenLocation.Y - Radius) && ScreenLocation.Y <= (ScreenLocation.Y + Radius);
	}

	bool InCircle(double Radius, FVector2D ScreenLocation)
	{
		if (InRect(Radius, ScreenLocation))
		{
			double dx = Variables::ScreenCenter.X - ScreenLocation.X; dx *= dx;
			double dy = Variables::ScreenCenter.Y - ScreenLocation.Y; dy *= dy;
			return dx + dy <= Radius * Radius;
		} return false;
	}

	/*
	if ( Settings::Aimbot::BoneType == 3 )
	{
									double MinDistance = DBL_MAX;

									for (const auto& Pair : HitscanBonePairs)
									{
										FVector WorldLocationFirst = Custom::GetSocketLocation(Mesh, Pair.first);
										FVector WorldLocationSecond = Custom::GetSocketLocation(Mesh, Pair.second);

										FVector2D ScreenPositionFirst = FVector2D();
										FVector2D ScreenPositionSecond = FVector2D();

										if (!Custom::K2_Project(WorldLocationFirst, &ScreenPositionFirst)) continue;
										if (!Custom::K2_Project(WorldLocationSecond, &ScreenPositionSecond)) continue;

										FVector2D DeltaFirst = FVector2D(ScreenPositionFirst.X - Variables::ScreenCenter.X, ScreenPositionFirst.Y - Variables::ScreenCenter.Y);
										double DistanceFirst = StaticClasses::KismetMathLibrary->sqrt(DeltaFirst.X * DeltaFirst.X + DeltaFirst.Y * DeltaFirst.Y);

										FVector2D DeltaSecond = FVector2D(ScreenPositionSecond.X - Variables::ScreenCenter.X, ScreenPositionSecond.Y - Variables::ScreenCenter.Y);
										double DistanceSecond = StaticClasses::KismetMathLibrary->sqrt(DeltaSecond.X * DeltaSecond.X + DeltaSecond.Y * DeltaSecond.Y);

										if (!StaticClasses::FortKismetLibrary->CheckLineOfSightToActorWithChannel(Variables::CameraLocation, Target, ECollisionChannel::ECC_Visibility, AcknowledgedPawn))
										{
											if (DistanceFirst < MinDistance && ViewportClient->LineTraceSingle(World, Variables::CameraLocation, WorldLocationFirst))
											{
												MinDistance = DistanceFirst;
												TargetBoneName = Pair.first;
											}

											if (DistanceSecond < MinDistance && ViewportClient->LineTraceSingle(World, Variables::CameraLocation, WorldLocationSecond))
											{
												MinDistance = DistanceSecond;
												TargetBoneName = Pair.second;
											}
										}
									}
								}
	
	*/
	
	// 	FString BoneTypes[]{ FString(L"Head"), FString(L"Neck"), FString(L"Root"), FString(L"Pelvis"), FString(L"Random"), FString(L"Closest") };
	FString GetAimBone(int AimBone)
	{
		if (AimBone == 1) // head
		{
			static auto Enc_AimboneNeck = Encrypt(L"neck_01");
			static auto AimboneNeck = FString(Enc_AimboneNeck.decrypt());
			return FString(AimboneNeck);
		}
		else if (Settings::Aimbot::BoneType == 2) // neck
		{
			static auto Enc_AimboneChest = Encrypt(L"spine_05");
			static auto AimboneChest = FString(Enc_AimboneChest.decrypt());
			return FString(AimboneChest);
		}
		else if (Settings::Aimbot::BoneType == 3) // root
		{
			static auto Enc_AimboneRoot = Encrypt(L"root");
			static auto AimboneRoot = FString(Enc_AimboneRoot.decrypt());
			return FString(Enc_AimboneRoot);
		}
		else if (Settings::Aimbot::BoneType == 4) // penis
		{
			static auto Enc_AimbonePelvis = Encrypt(L"pelvis");
			static auto AimbonePelvis = FString(Enc_AimbonePelvis.decrypt());
			return FString(AimbonePelvis);
		}
		else if (Settings::Aimbot::BoneType == 5) // random
		{
			static double LastBoneChangeTime = 0.00;
			int BoneIndex = 0;
			FString TargetBone = FString(Encrypt(L"neck_01").decrypt());

			const FString BoneArray[] =
			{
				FString(Encrypt(L"neck_01").decrypt()),
				FString(Encrypt(L"neck_02").decrypt()),
				FString(Encrypt(L"spine_01").decrypt()),
				FString(Encrypt(L"spine_02").decrypt()),
				FString(Encrypt(L"spine_03").decrypt()),
				FString(Encrypt(L"clavicle_r").decrypt()),
				FString(Encrypt(L"clavicle_l").decrypt())
			};

			if (Variables::World)
			{

				if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(Variables::World) - LastBoneChangeTime >= 1.70)
				{
					int32_t RandomIndex = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 6); // 7 possible
					TargetBone = BoneArray[BoneIndex];
					LastBoneChangeTime = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(Variables::World);
				}
			}
			else
			{
				int32_t RandomIndex = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 6);
				TargetBone = BoneArray[RandomIndex];
				LastBoneChangeTime = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(Variables::World);
			}

			return TargetBone;
		}

		static auto Enc_AimboneChest = Encrypt(L"spine_05"); // fallback to neck if given bad data.
		static auto AimboneChest = FString(Enc_AimboneChest.decrypt());
		return FString(AimboneChest);
	}

	bool IsHitscanBone(int AimBone) { return AimBone == 5; };

	FLinearColor GetColorByTier(EFortRarity Tier)
	{
		switch (Tier)
		{
		case EFortRarity::Common:
			return FLinearColor(0.4f, 0.4f, 0.4f, 1.f);
		case EFortRarity::Uncommon:
			return FLinearColor(0.2f, 1.f, 0.2f, 1.f);
		case EFortRarity::Rare:
			return FLinearColor(0.2f, 0.2f, 0.9f, 1.f);
		case EFortRarity::Epic:
			return FLinearColor(1.f, 0.2f, 1.f, 1.f);
		case EFortRarity::Legendary:
			return FLinearColor(1.f, 1.f, 0.2f, 1.f);
		case EFortRarity::Mythic:
			return FLinearColor(1.f, 0.84f, 0.f, 1.f);
		default:
			return FLinearColor(0.4f, 0.4f, 0.4f, 1.f);
		}
	}

	FString GetNameByPlatform(FString Platform)
	{
		if (Platform == FString(L"WIN"))
			return FString(L"Windows");
		else if (Platform == FString(L"XBL"))
			return FString(L"Xbox One");
		else if (Platform == FString(L"XSX"))
			return FString(L"Xbox Series X/S");
		else if (Platform == FString(L"PSN") || Platform == FString(L"PS5"))
			return FString(L"PlayStation");
		else if (Platform == FString(L"SWT"))
			return FString(L"Nintendo Switch");
		else if (Platform == FString(L"AND"))
			return FString(L"Android");
		else if (Platform == FString(L"MAC"))
			return FString(L"MacOS");
		else if (Platform == FString(L"LNX"))
			return FString(L"Linux");
		else if (Platform == FString(L"IOS"))
			return FString(L"Apple");
		return FString(L"BOT/AI");
	}

	FLinearColor GetColorByPlatform(FString Platform)
	{
		if (Platform == FString(L"WIN"))
			return FLinearColor(1.f, 1.f, 1.f, 1.f);
		else if (Platform == FString(L"XBL"))
			return FLinearColor(0.0627f, 0.4863f, 0.0627f, 1.0f);
		if (Platform == FString(L"PSN") || Platform == FString(L"PS5"))
			return FLinearColor(0.f, 0.188f, 0.529f, 1.f);
		else if (Platform == FString(L"SWT"))
			return FLinearColor(0.902f, 0.0f, 0.0706f, 1.f);
		else if (Platform == FString(L"AND"))
			return FLinearColor(0.2392f, 0.8627f, 0.5176f, 1.f);
		else if (Platform == FString(L"MAC"))
			return FLinearColor(0.5569f, 0.5569f, 0.5765f, 1.f);
		else if (Platform == FString(L"LNX"))
			return FLinearColor(1.f, 0.843f, 0.f, 1.f);
		else if (Platform == FString(L"IOS"))
			return FLinearColor(0.f, 0.4784f, 1.f, 1.f);
		return FLinearColor(1.f, 0.f, 0.f, 1.f);
	}

	FString GetRankByProgress(int32_t Rank)
	{
		switch (Rank)
		{
		case 0:
		case 1:
		case 2:
			return FString(L"T-UI-Icon-Habenaro-Bronze");
		case 3:
		case 4:
		case 5:
			return FString(L"T-UI-Icon-Habenaro-Silver");
		case 6:
		case 7:
		case 8:
			return FString(L"T-UI-Icon-Habenaro-Gold");
		case 9:
		case 10:
		case 11:
			return FString(L"T-UI-Icon-Habenaro-Platinum");
		case 12:
		case 13:
		case 14:
			return FString(L"T-UI-Icon-Habenaro-Diamond");
		case 15:
			return FString(L"T-UI-Icon-Habenaro-Elite");
		case 16:
			return FString(L"T-UI-Icon-Habenaro-Champion");
		case 17:
			return FString(L"T-UI-Icon-Habenaro-Unreal");
		default:
			return FString(L"None");
		}
	}

	FLinearColor GetColorByRank(int32_t Rank)
	{
		switch (Rank)
		{
		case 0:
		case 1:
		case 2:
			return FLinearColor(0.902f, 0.580f, 0.227f, 1.0f);
		case 3:
		case 4:
		case 5:
			return FLinearColor(0.843f, 0.843f, 0.843f, 1.0f);
		case 6:
		case 7:
		case 8:
			return FLinearColor(1.0f, 0.871f, 0.0f, 1.0f);
		case 9:
		case 10:
		case 11:
			return FLinearColor(0.0f, 0.7f, 0.7f, 1.0f);
		case 12:
		case 13:
		case 14:
			return FLinearColor(0.0f, 0.0f, 0.7f, 1.0f);
		case 15:
			return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
		case 16:
			return FLinearColor(1.0f, 0.6f, 0.0f, 1.0f);
		case 17:
			return FLinearColor(0.6f, 0.0f, 0.6f, 1.0f);
		default:
			return FLinearColor(0.0f, 0.7859f, 1.0f, 1.0f);
		}
	}

	FString GetBuildByType(EFortBuildingType Build)
	{
		switch (Build)
		{
		case EFortBuildingType::Wall:
			return FString(L"Wall");
		case EFortBuildingType::Floor:
			return FString(L"Floor");
		case EFortBuildingType::Corner:
			return FString(L"Corner");
		case EFortBuildingType::Deco:
			return FString(L"Deco");
		case EFortBuildingType::Prop:
			return FString(L"Prop");
		case EFortBuildingType::Stairs:
			return FString(L"Stairs");
		case EFortBuildingType::Roof:
			return FString(L"Roof");
		case EFortBuildingType::Pillar:
			return FString(L"Pillar");
		case EFortBuildingType::SpawnedItem:
			return FString(L"Spawned Item");
		case EFortBuildingType::Container:
			return FString(L"Container");
		case EFortBuildingType::Trap:
			return FString(L"Trap");
		case EFortBuildingType::GenericCenterCellActor:
			return FString(L"Generic Center Cell Actor");
		case EFortBuildingType::StructuralCorner:
			return FString(L"Structural Corner");
		case EFortBuildingType::None:
		default:
			return FString(L"None");
		}
	}

	// add to the cache, default at 2.0, allows returning modified version, but defaults to returning the unmodified one, used internally by GetDebugLoc
	double AddOffsetText(double Spacing = 2.0, bool ReturnModifiedVersion = false)
	{
		double OriginalOffset = Settings::Developer::Debugging::TextOffset;
		double NewOffset = OriginalOffset;
		NewOffset += (Settings::Misc::FontSize + Spacing); // append the new siz
		
		Settings::Developer::Debugging::TextOffset = NewOffset;

		if (ReturnModifiedVersion)
		{
			return Settings::Developer::Debugging::TextOffset;
		}

		return OriginalOffset;
	}

	// reset so we don't spam down past the ui -> you should call this each render of a debug ui
	void InitDebugUI()
	{
		Settings::Developer::Debugging::TextOffset = 0;
	}

	// get debug ui offset text -> used internally by GetDebugLoc
	double GetCurrentOffsetText() 
	{
		return Settings::Developer::Debugging::TextOffset;
	}

	bool IsDebugUIInProgress()
	{

	}

	// get debug ui position on screen for current element, allows handling adding values so our ui doesn't become even worse shitcode
	FVector2D GetDebugLoc(bool AppendOffset = true, double OffsetToAppend = 2.0)
	{
		if (!AppendOffset)
		{
			return FVector2D(5, 5 + Custom::GetCurrentOffsetText());
		}

		return FVector2D(5, 5 + Custom::AddOffsetText(OffsetToAppend)); // get the original lol
	}

	// begin debug ui -> optional watermark
	bool DebugUIBegin(FString Watermark = L"conspiracy :3\r\nUI Debug Mode -> Vixen.\r\n", FLinearColor WatermarkColor = FLinearColor(0.f, 1.0f, 0.f, 1.f))
	{
		//if (!Wrapper::IsSafeToRender()) return false; // return false since this failed
		// i commented this since idk if itll work lol ^
		if (Settings::Developer::Debugging::TextOffset != 0) return false; // failed due to it already rendering or due to user not calling DebugUIEnd
		
		InitDebugUI();

		Wrapper::Text(FString(Watermark), Custom::GetDebugLoc(), FLinearColor(1.f, 1.f, 1.f, 1.f));

		if (Settings::Developer::Debugging::TextOffset == 0) return false; // multiple rendering at once possibly, fail to be safe and let new one work.

		return true; // successful init!
	}

	// bool so we can do stuff at the end if you want i guess
	bool DebugUIEnd()
	{
		Settings::Developer::Debugging::TextOffset = 0;
		return (Settings::Developer::Debugging::TextOffset == 0); // should always return true but good to return false in case of multiple uis running or something weird
	}

	// used internally by WriteDebugUIStringEx -> gets FLinearColor via message type
	FLinearColor GetVColor(VixenDebugMessageType ColorCodeType_)
	{
		if (ColorCodeType_ == VixenDebugMessageType::Log) // white
			return FLinearColor(1.f, 1.f, 1.f, 1.f); // white
		if (ColorCodeType_ == VixenDebugMessageType::Info) // white
			return FLinearColor(1.f, 1.f, 1.f, 1.f); // white
		if (ColorCodeType_ == VixenDebugMessageType::Warn) // orange
			return FLinearColor(1.f, 0.5f, 0.f, 1.f); // orange
		if (ColorCodeType_ == VixenDebugMessageType::Error) // red
			return FLinearColor(1.f, 0.0f, 0.f, 1.f); // red
		if (ColorCodeType_ == VixenDebugMessageType::Debug) // green
			return FLinearColor(0.f, 1.0f, 0.f, 1.f); // green
		if (ColorCodeType_ == VixenDebugMessageType::Misc) // cyan
			return FLinearColor(0.f, 1.0f, 1.f, 1.f); // cyan
		if (ColorCodeType_ == VixenDebugMessageType::Other) // cyan
			return FLinearColor(0.f, 1.0f, 1.f, 1.f); // cyan

		return FLinearColor(1.f, 1.f, 1.f, 1.f); // white if we have no idea what color
	}

	// write string with color
	void WriteDebugUIString(FString Content, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f))
	{
		Wrapper::Text(FString(Content), Custom::GetDebugLoc(), Color);
		return;
	}

	// write string via message type (VixenDebugMessageType)
	void WriteDebugUIStringEx(FString Content, VixenDebugMessageType ColorCodeType_)
	{
		Custom::WriteDebugUIString(Content, GetVColor(ColorCodeType_)); // fud wrapper :sob:
		return;
	}

	// writes double
	void WriteDebugUIString_Double(FString Content, double DebugValue, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f))
	{
		FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(FString(Content), FString(L" "), DebugValue, FString(L""));
		Wrapper::Text(ConvertedText, Custom::GetDebugLoc(), Color);
		return;
	}

	// writes float
	void WriteDebugUIString_Float(FString Content, float DebugValue, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f))
	{
		WriteDebugUIString_Double(Content, DebugValue, Color); // fud
		return;
	}

	// writes int
	void WriteDebugUIString_Int(FString Content, int DebugValue, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f))
	{
		FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Int(Content, FString(L" "), DebugValue, FString(L""));
		Wrapper::Text(ConvertedText, Custom::GetDebugLoc(), Color);
		return;
	}

	// writes bool
	void WriteDebugUIString_Bool(FString Content, bool DebugValue, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f))
	{
		WriteDebugUIString_Int(Content, DebugValue, Color); // fud
		return;
	}

	// converts Float time to string and then writes it as debug
	void WriteDebugUIString_StringifiedTimeByFloat(FString Content, float DebugValue, FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f))
	{
		FString StringTime = StaticClasses::KismetStringLibrary->TimeSecondsToString(DebugValue);

		FString ConvertedText = StaticClasses::KismetStringLibrary->Concat_StrStr(Content, StringTime);

		WriteDebugUIString(ConvertedText, Color); // nice
		return;
	}

	// continue if the value true so we can do ts easier
	bool DebugState(bool State)
	{
		return (State == true); // ud
	}
}