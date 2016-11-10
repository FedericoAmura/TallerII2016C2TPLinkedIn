package com.example.android.clientapp;
/**
 * Created by guidonegri on 05/11/16.
 */

import android.content.Context;
import android.content.Intent;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.example.android.clientapp.Modelo.Amigo;

import java.util.List;

public class RVAdapter extends RecyclerView.Adapter<RVAdapter.PersonViewHolder> {

    public static class PersonViewHolder extends RecyclerView.ViewHolder {

        CardView cv;
        TextView personName;
        TextView personAge;
        ImageView personPhoto;
        String id;

        PersonViewHolder(View itemView, final Class clase) {
            super(itemView);
            cv = (CardView)itemView.findViewById(R.id.cv);
            personName = (TextView)itemView.findViewById(R.id.person_name);
            personAge = (TextView)itemView.findViewById(R.id.person_age);
            personPhoto = (ImageView)itemView.findViewById(R.id.person_photo);
            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Context context = v.getContext();
                    Intent intent = new Intent(context, clase);
                    intent.putExtra("userID", id);
                    context.startActivity(intent);
                }
            });
        }
    }

    List<Amigo> amigos;
    Class clase;

    public RVAdapter(List<Amigo> amigos, Class clase){
        this.amigos = amigos;
        this.clase = clase;
    }

    @Override
    public void onAttachedToRecyclerView(RecyclerView recyclerView) {
        super.onAttachedToRecyclerView(recyclerView);
    }

    @Override
    public PersonViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        View v = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.fragment_relaciones, viewGroup, false);
        PersonViewHolder pvh = new PersonViewHolder(v, clase);
        return pvh;
    }

    @Override
    public void onBindViewHolder(PersonViewHolder personViewHolder, int i) {
        personViewHolder.personName.setText(amigos.get(i).getNombre());
        personViewHolder.personAge.setText(amigos.get(i).getCiudad());
        personViewHolder.personPhoto.setImageBitmap(amigos.get(i).getFoto());
        personViewHolder.id = amigos.get(i).getUserID();
    }

    @Override
    public int getItemCount() {
        return amigos.size();
    }
}